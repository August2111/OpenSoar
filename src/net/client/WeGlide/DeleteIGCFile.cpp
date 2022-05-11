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

#include "DeleteIGCFile.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "LogFile.hpp"
#include "DeleteFlight.hpp"
// in Header... #include "Settings.hpp"
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

namespace WeGlide {

// static Flight
// UploadJsonInterpreter(const boost::json::value &json)
// {
//   Flight flight_data;
//   // flight is the 1st flight object in this array ('at(0)')
//   auto flight = json;
//     // .as_array().at(0);
//   flight_data.scoring_date = GetJsonString(flight, "scoring_date").c_str();
//   flight_data.flight_id = flight.at("id").to_number<int64_t>();
//   flight_data.registration = GetJsonString(flight, "registration").c_str();
//   flight_data.competition_id = GetJsonString(flight, "competition_id").c_str();
// 
//   auto user = flight.at("user").as_object();
//   flight_data.user.id = user.at("id").to_number<uint32_t>();
//   flight_data.user.name = GetJsonString(user, "name").c_str();
// 
//   auto aircraft = flight.at("aircraft").as_object();
//   flight_data.aircraft.id = aircraft.at("id").to_number<uint32_t>();
//   flight_data.aircraft.name = GetJsonString(aircraft, "name").c_str();
//   flight_data.aircraft.kind = GetJsonString(aircraft, "kind").c_str();
//   flight_data.aircraft.sc_class = GetJsonString(aircraft, "sc_class").c_str();
// 
//   return flight_data;
// }

static const StaticString<0x100>
DeleteErrorInterpreter(const HttpResponse &http)
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
  Co::InvokeTask DeleteTask(const User &user, uint_least32_t flight_id,
                           ProgressListener &progress) {
    http = co_await DeleteFlight(*Net::curl, user, flight_id, progress);
  }
};

static bool
DeleteFlight(User user, uint_least32_t flight_id,
               StaticString<0x1000> &msg) noexcept
try {
  const auto settings = CommonInterface::GetComputerSettings();
  if (user.id == 0 || user.id == settings.weglide.pilot.id)
    user = settings.weglide.pilot;

  #if 0
  CoInstance instance;
  if (instance.DeleteTask(user, flight_id) == false) {
  }
  if (instance.http.code >= 200 && instance.http.code < 400 &&
#else

  PluggableOperationEnvironment env; 
  CoInstance instance;

  if (ShowCoDialog(UIGlobals::GetMainWindow(),
                          UIGlobals::GetDialogLook(), _("Delete Flight"),
                          instance.DeleteTask(user, flight_id, env),
                          &env) == false) {
    msg.Format(_("'%s' - %s"), _("Delete Flight"),
               _("ShowCoDialog with failure"));
  } else if (instance.http.code >= 200 && instance.http.code < 400 &&
#endif
      1) {

    msg.Format(_("File delete '%u' was successful"), flight_id);
    return true; // upload successful!
  } else {
    msg = DeleteErrorInterpreter(instance.http);
  }
  return false; // failure...
} catch (const std::exception &e) {
  msg.Format(_T("'%s' - %s"), _T(""),  // igc_path.c_str(),
    UTF8ToWideConverter(e.what()).c_str());
  return false;
} catch (...) {
  msg.Format(_T("'%s' - %s"), _("General Exception"), _T(""));  // igc_path.c_str());
  ShowError(std::current_exception(), _T("WeGlide DeleteFile"));
  return false;
}

bool
DeleteIGCFile(const User &user, uint_least32_t flight_id) noexcept try {
  StaticString<0x1000> msg;
  auto flight_data = DeleteFlight(user, flight_id, msg);
  if (flight_data) {  // }.IsValid()) {
    // upload successful!
// !!!!    FlightUploadResponse(flight_data, msg.c_str());
    ShowMessageBox(msg.c_str(), _("WeGlide Delete"), MB_OK);
    return true;
  } else {
    // upload failed!
    // DeleteFlight();
    LogFormat(_T("%s: %s!"), _("WeGlide Delete Error"), msg.c_str());
    ShowMessageBox(msg.c_str(), _("WeGlide Delete Error"), MB_ICONEXCLAMATION);
  }
  return false;
} catch (...) {
  ShowError(std::current_exception(), _T("WeGlide DeleteIGCFile"));
  return false;
}

} // namespace WeGlide
