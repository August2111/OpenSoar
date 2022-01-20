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
#include "LocalPath.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "LogFile.hpp"
#include "Cloud/weglide/UploadFlight.hpp"
#include "Cloud/weglide/WeGlideSettings.hpp"
#include "co/InvokeTask.hxx"
#include "Dialogs/Message.hpp"
#include "Dialogs/CoDialog.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "json/ParserOutputStream.hxx"
#include "Language/Language.hpp"
#include "net/http/Init.hpp"
#include "Operation/PluggableOperationEnvironment.hpp"
#include "system/ConvertPathName.hpp"
#include "system/FileUtil.hpp"
#include "util/StaticString.hxx"
#include "util/ConvertString.hpp"
#include "Message.hpp"

#include <cinttypes>

/**
 * Wrapper for getting string values of a json value
 *
 * @param value - the json object value
 * @param key   - the key of the wanted value as string
 * @param error_code - the return value in case of faulty json object
 *
 * @return the converted string value or the error_code (in case of an error)
 */

static const UTF8ToWideConverter 
GetJsonString(boost::json::standalone::value json_value, std::string_view key,
              const char* error_code = "") noexcept {
  auto value = json_value.at(key);
  const auto value_str = (value.is_string() ?
    value.get_string().c_str() : error_code);
  // const UTF8ToWideConverter value_str2(value_str);
  return UTF8ToWideConverter(value_str); //  UTF8ToWideConverter(value_str.str());
}

namespace WeGlide {

struct User {
  uint32_t id = 0;
  BrokenDate birthdate;
  StaticString<0x80> name;
};

struct Aircraft {
  uint32_t id = 0;
  StaticString<0x40> name;
  StaticString<4> kind;  // 'MG' - motor aircraft,...
  StaticString<10> sc_class;
};

struct UploadResponse {
  uint64_t flight_id = 0;
  User user;
  Aircraft aircraft;
  StaticString<0x40> scoring_date;
  StaticString<0x40> registration;
  StaticString<0x40> competition_id;
};

static void
UploadJsonInterpreter(const boost::json::value &json,
                      UploadResponse& response) {
  if (json.is_array() && (json.get_array().size() > 0)) {
    auto flight_obj = json.get_array().at(0);
    response.scoring_date = GetJsonString(flight_obj, "scoring_date").c_str();
    response.flight_id = flight_obj.at("id").to_number<int64_t>();
    response.registration = GetJsonString(flight_obj, "registration").c_str();
    response.competition_id = GetJsonString(flight_obj, "competition_id").c_str();
    auto user = flight_obj.at("user");
    if (user.is_object()) {
      auto user_obj = user.as_object();
      response.user.id = user_obj.at("id").to_number<uint32_t>();
      response.user.name = GetJsonString(user_obj, "name").c_str();
    }
    auto aircraft = flight_obj.at("aircraft");
    if (aircraft.is_object()) {
      auto aircraft_obj = aircraft.as_object();
      response.aircraft.id = aircraft_obj.at("id").to_number<uint32_t>();
      response.aircraft.name = GetJsonString(aircraft_obj, "name").c_str();
      response.aircraft.kind = GetJsonString(aircraft_obj, "kind").c_str();
      response.aircraft.sc_class = GetJsonString(aircraft_obj, "sc_class").c_str();
    }
  }
  else {
    throw std::runtime_error("The http response has a wrong JSON format!");
  }
}

static void
UploadSuccessDialog(const UploadResponse& response, const Path igc_path) {
  // TODO: Create a real Dialog with fields
  // With this Dialog insert the possibilty to update/patch the flight
  // f.e. copilot in double seater, scoring class, short comment and so on
  StaticString<0x1000> msg;
  msg.Format(_("File upload '%s' was succesfull"),
    igc_path.c_str());
  msg.append(_T("!\n\n"));
  msg.AppendFormat(_T("%-20s: %u\n"), _("flight_id"), response.flight_id);
  msg.AppendFormat(_T("%-20s: %s\n"), _("scoring_date"),
    response.scoring_date.c_str());
  msg.AppendFormat(_T("%-20s: %s (%d)\n"), _("user"),
    response.user.name.c_str(), response.user.id);
  msg.AppendFormat(_T("%-20s: %s (%u)\n"), _("aircraft type"),
    response.aircraft.name.c_str(), response.aircraft.id);
  msg.AppendFormat(_T("%-20s: %s: %s "), _("aircraft"), _("reg"),
    response.registration.c_str());
  msg.AppendFormat(_T("%s: %s\n"), _("cid"),
    response.competition_id.c_str());

  Message::AddMessage(msg, 10000);
}

static Co::InvokeTask
UpdateTask(Path igc_path, const char* url, const User& user,
  uint_least32_t glider_id, boost::json::value& json,
  ProgressListener& progress) {
  json = co_await UploadFlight(*Net::curl, url, user, glider_id,
    igc_path, progress);
}

bool
UploadIGCFile(Path filepath, Pilot pilot,
              uint_least32_t glider_id) noexcept {
  NarrowString<0x200> url(WeGlideSettings::default_url);
  url += "/igcfile";
  auto igc_path = filepath.IsAbsolute() ? AllocatedPath(filepath) :
    LocalPath(filepath);

  try {
    WeGlideSettings settings = CommonInterface::GetComputerSettings().weglide;
    if (glider_id == 0)
      glider_id = CommonInterface::GetComputerSettings().plane
      .weglide_glider_type;
    if (pilot.id == 0) {
      pilot = CommonInterface::GetComputerSettings().weglide.pilot;
    }

    LogFormat(_("WeGlide Upload: %s"), igc_path.c_str());
    if (!File::Exists(igc_path)) {
      StaticString<0x200> msg;
      msg.Format(_("Error! File '%s' doesn't exist!"), igc_path.c_str());
      LogFormat(_("WeGlide Upload Error: %s"), msg.c_str());
      ShowMessageBox(msg, _("WeGlide Upload"), MB_OK | MB_ICONEXCLAMATION);

      return false;
    }

    UploadResponse response;
    PluggableOperationEnvironment env;
    boost::json::value json;
    if (ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
      _("Upload Flight"), UpdateTask(igc_path, url.c_str(), pilot,
        glider_id, json, env), &env) == false)
    {
      return false;
    }
    
    // read the important data from json in a structure
    UploadJsonInterpreter(json, response);

    // Show the response from WeGlide server after upload in a very simple view
    UploadSuccessDialog(response, igc_path);

    return true;  // upload succesful!
  }
  catch (const std::exception& e) {
    // Check for errors
    StaticString<0x1000> msg(_("WeGlide Upload"));
    msg.append(_T(": "));
    msg.append(UTF8ToWideConverter(e.what()).c_str());
    msg.append(_("\n"));
    msg.append(igc_path.c_str());

    LogFormat(_("WeGlide Upload Error: %s"), msg.c_str());
    Message::AddMessage(msg, 10000);
    return false;
  }
}

} // namespace WeGlide
