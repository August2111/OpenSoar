/*
  Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "PatchIGCFile.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "LogFile.hpp"
#include "PatchFlight.hpp"
#include "Settings.hpp"
#include "HttpResponse.hpp"
#include "GetJsonString.hpp"
#include "co/InvokeTask.hxx"
#include "Dialogs/Message.hpp"
#include "Dialogs/CoDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Dialogs/Contest/WeGlide/FlightUploadResponse.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "json/ParserOutputStream.hxx"
#include "Language/Language.hpp"
#include "net/http/Init.hpp"
#include "Operation/PluggableOperationEnvironment.hpp"
#include "system/ConvertPathName.hpp"
#include "system/FileUtil.hpp"
#include "util/StaticString.hxx"
#include "util/ConvertString.hpp"

#include <cinttypes>
#include <sstream>

// #include <boost/property_tree/json_parser.hpp>
// #include <boost/property_tree/ptree.hpp>
// #include <iostream>
// #include <sstream>


namespace WeGlide {

static Flight
PatchJsonInterpreter(const boost::json::value &json)
{
  // boost::property_tree::ptree tree;
  std::stringstream ss;
  ss << json;
  // boost::property_tree::write_json(ss, tree);
//  auto _str = ss.str();
  LogFormat("JSON: %s", ss.str().c_str());

//  if (json.is_null())
//    LogFormat("JSON: empty");
//  else {
//    auto str = boost::json::value_to<std::string>(json);
//    LogFormat("JSON: %s", str.c_str());
//  }

  Flight flight_data;
  flight_data.igc_name.clear();  // no IGC name at patch!
  // flight is the 1st flight object in this array ('at(0)')
  auto flight = json;
    // .as_array().at(0);
  flight_data.scoring_date = GetJsonString(flight, "scoring_date").c_str();
  flight_data.flight_id = flight.at("id").to_number<int64_t>();
  flight_data.registration = GetJsonString(flight, "registration").c_str();
  flight_data.competition_id = GetJsonString(flight, "competition_id").c_str();

  auto user = flight.at("user").as_object();
  flight_data.user.id = user.at("id").to_number<uint32_t>();
  flight_data.user.name = GetJsonString(user, "name").c_str();

  auto aircraft = flight.at("aircraft").as_object();
  flight_data.aircraft.id = aircraft.at("id").to_number<uint32_t>();
  flight_data.aircraft.name = GetJsonString(aircraft, "name").c_str();
  flight_data.aircraft.kind = GetJsonString(aircraft, "kind").c_str();
  flight_data.aircraft.sc_class = GetJsonString(aircraft, "sc_class").c_str();

  return flight_data;
}

static const StaticString<0x100>
PatchErrorInterpreter(const HttpResponse &http)
{
  auto error = http.json_value;
  StaticString<0x100> error_string;
  error_string.Format(_T("%s: %u"), _("HTTP failure code"), http.code);
  if (!error.is_null()) {
    StaticString<0x40> error_code(_T(""));
    try {
      if (error.at("error_description").is_string())
        error_code = GetJsonString(error, "error_description").c_str();
    } catch ([[maybe_unused]] std::exception &e) {
    }
    if (error_code.empty()) {
      try {
        if (error.at("error").is_string())
          error_code = GetJsonString(error, "error_description").c_str();
      } catch ([[maybe_unused]] std::exception &e) {
      }
    }
    if (!error_code.empty()) {
      error_string.AppendFormat(_("\n%s: "), _("Error Description"));
      error_string += error_code;
    }
  }
  return error_string;
}

struct CoInstance {
  HttpResponse http;
  Co::InvokeTask PatchTask(const User &user, uint_least32_t flight_id,
                           const boost::json::value &patch,
                           ProgressListener &progress) {
    http = co_await PatchFlight(*Net::curl, user, flight_id, patch, progress);
  }
};

// static
Flight PatchIGCFlight(Flight &flight,
               const boost::json::value &patch,
               StaticString<0x1000> &msg) noexcept
try {
  const auto settings = CommonInterface::GetComputerSettings();
  if (flight.user.id == 0 || flight.user.id == settings.weglide.pilot.id)
    flight.user = settings.weglide.pilot;

  PluggableOperationEnvironment env;
  CoInstance instance;
#if 0
  if (instance.PatchTask(user, flight_id, patches, env) == false) {
  }
  if (instance.http.code >= 200 && instance.http.code < 400 &&
#else
  boost::json::object json_obj;


  if (ShowCoDialog(UIGlobals::GetMainWindow(),
                   UIGlobals::GetDialogLook(), _("Patch Flight"),
                   instance.PatchTask(flight.user, flight.flight_id, patch,
                   env), &env) == false) {
    msg.Format(_("'%s' - %s"), _("Patch Flight"),
               _("ShowCoDialog with failure"));
  } else if (instance.http.code >= 200 && instance.http.code < 400 &&
#endif
      !instance.http.json_value.is_null()) {

    // read the important data from json in a structure
    auto flight_data = PatchJsonInterpreter(instance.http.json_value);
    flight_data.igc_name = flight.igc_name;

    if (msg.empty())  // if msg isn't empty!
      msg.Format(_("File patch '%u' was successful"), flight.flight_id);
    return flight_data; // upload successful!
  } else {
    msg = PatchErrorInterpreter(instance.http);
  }
  return Flight();  // failure...
} catch (const std::exception &e) {
  msg.AppendFormat(_T("\n'%s: %u' - %s: %u"), _T("Patch Flight: "),
                   flight.flight_id, UTF8ToWideConverter(e.what()).c_str());
  return Flight();
} catch (...) {
  msg.AppendFormat(_T("\n'%s: %u' - %s: %u"), _T("Patch Flight: "),
                   flight.flight_id, _("General Exception") );
  ShowError(std::current_exception(), _T("WeGlide PatchFile"));
  return Flight();
}

bool
PatchIGCFile(const User &user, uint_least32_t flight_id,
                  const boost::json::value &patch) noexcept try {
  StaticString<0x1000> msg;
  Flight flight;
  flight.user = user;
  flight.flight_id = flight_id;
  auto flight_data = PatchIGCFlight(flight, patch, msg);
  if (flight_data.IsValid()) {
    // upload successful!
// !!!!
    FlightUploadResponse(flight_data, msg.c_str());
    return true;
  } else {
    // upload failed!
    // PatchFlight();
    LogFormat(_T("%s: %s!"), _("WeGlide Patch Error"), msg.c_str());
    ShowMessageBox(msg.c_str(), _("WeGlide Patch Error"), MB_ICONEXCLAMATION);
  }
  return false;
} catch (...) {
  ShowError(std::current_exception(), _T("WeGlide PatchIGCFile"));
  return false;
}

} // namespace WeGlide
