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

#include "UploadIGCFile.hpp"
#include "UploadFlight.hpp"
#include "PatchIGCFile.hpp"
#include "WeGlideObjects.hpp"
#include "HttpResponse.hpp"
#include "GetJsonString.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "LogFile.hpp"
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

namespace WeGlide {

static Flight
UploadJsonInterpreter(const boost::json::value &json)
{
  Flight flight_data;
  // flight is the 1st flight object in this array ('at(0)')
  auto flight = json.as_array().at(0);
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

static const boost::json::value
SetJsonData() {
  Json::ParserOutputStream parser;
  std::stringstream ss;
  ss << "{";
  ss << "\"comment\": \"Uploaded via XCSoar!\", ";
  // maybe set the competion id from the setting here?
  ss << "\"competition_id\": \"\", ";
  // if aircraft -> double_seater:
  //    ss << "\"co_user_name\": \"Copilot No1\", ";
  ss << "\"rescore\": false ";
  ss << "}";
  parser.Write(ss.str().c_str(), ss.str().length());
  return parser.Finish();
}

#if 0  // not used up to now
static const StaticString<0x100>
UploadErrorInterpreter(const HttpResponse &http)
{
  auto error = http.json_value;
  StaticString<0x100> error_string;
  error_string.Format(_T("%s: %u"), _("HTTP failure code"), http.code);
  if (!error.is_null()) {
    StaticString<0x40> error_code;
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
#endif

struct CoInstance {
  HttpResponse http;
  Co::InvokeTask
  UpdateTask(Path igc_path, const User &user,
    const Aircraft &aircraft, ProgressListener &progress)
  {
    http = co_await UploadFlight(*Net::curl, user, aircraft,
      igc_path, progress);
  }
};

static Flight
UploadFile(Path igc_path, User user, Aircraft aircraft,
  StaticString<0x1000> &msg) noexcept
{
  Flight flight_data;
  try {
    if (!aircraft.IsValid())
      aircraft = Aircraft({CommonInterface::GetComputerSettings().plane
        .weglide_glider_type});
    if (!user.IsValid()) {
      user = CommonInterface::GetComputerSettings().weglide.pilot;
    }

    if (!File::Exists(igc_path)) {
      msg.Format(_T("'%s' - %s"), igc_path.c_str(), _("Not found"));
      return flight_data;  // with flight_id = 0!
    }

    PluggableOperationEnvironment env;
    CoInstance instance;
    if (ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
      _("Upload Flight"), instance.UpdateTask(igc_path, user,
        aircraft, env), &env) == false) {
      msg.Format(_T("'%s' - %s"), igc_path.c_str(),
        _("ShowCoDialog with failure"));
      return flight_data;  // with flight_id = 0!
    }

    if (instance.http.code >= 200 && instance.http.code < 400 &&
      !instance.http.json_value.is_null()) {

      // read the important data from json in a structure
      flight_data = UploadJsonInterpreter(instance.http.json_value);
      flight_data.igc_name = igc_path.GetBase().c_str();

      msg.Format(_("File upload '%s' was successful"),
                 flight_data.igc_name.c_str());
    } else {
      msg.Format(_T("%s: %u"), _("HTTP failure code"), instance.http.code);
    }
    return flight_data;  // upload successful!
  }
  catch (const std::exception &e) {
    msg.Format(_T("'%s' - %s"), igc_path.c_str(),
      UTF8ToWideConverter(e.what()).c_str());
  }
  return Flight();  // failure...
}

bool
UploadIGCFile(Path igc_path, const User &user,
  const Aircraft &aircraft) noexcept { 
  try {
    StaticString<0x1000> msg;
    // ??? UploadSuccessDialog(flight_data, msg.c_str());
    auto flightdata = UploadFile(igc_path, user, aircraft, msg);
    if (flightdata.IsValid()) {
      // upload successful!
      auto json = SetJsonData();
      if (!user.token.empty())  // patch only possible with user token!
        flightdata = PatchIGCFlight(flightdata, json, msg);
      FlightUploadResponse(flightdata, msg.c_str());
      return true;
    } else {
      // upload failed!
      LogFormat(_T("%s: %s"), _("WeGlide Upload Error"), msg.c_str());
      ShowMessageBox(msg.c_str(), _("WeGlide Upload Error"), 
                     MB_ICONEXCLAMATION);
    }
  } catch (...) {
	ShowError(std::current_exception(), _("WeGlide UploadIGCFile"));
  }
  return false;
}

} // namespace WeGlide
