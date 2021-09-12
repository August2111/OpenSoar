/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
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

#include "WeGlideUploadDialog.hpp"
#include "Interface.hpp"
#include "Cloud/weglide/WeGlideSettings.hpp"
#include "Cloud/weglide/UploadFlight.hpp"
#include "Operation/PluggableOperationEnvironment.hpp"
#include "net/http/Init.hpp"
#include "json/ParserOutputStream.hxx"
#include "Dialogs/CoDialog.hpp"
#include "Dialogs/Error.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"
#include "co/InvokeTask.hxx"
#include "Dialogs/Message.hpp"
#include "util/Exception.hxx"
#include "util/ConvertString.hpp"
#include "io/StringConverter.hpp"
#include "LogFile.hpp"
#include "json/Lookup.hxx"
#include "system/Path.hpp"
#include "LocalPath.hpp"

#include <inttypes.h>

namespace WeGlide {

typedef struct _Glider {
  uint64_t id;
  StaticString<0x40> name;
  StaticString<4> kind;  // 'MG' - motor glider,...
  StaticString<10> sc_class;
  uint64_t seats;
} Glider;

typedef struct _UploadResponse {
  uint64_t flight_id;
  Pilot pilot;
  Glider glider;
  // StaticString<0x40> pilot_name;
  StaticString<0x40> scoring_date;
  StaticString<0x40> registration;
  StaticString<0x40> competition_id;
} UploadResponse;

static Co::InvokeTask UpdateTask(Path igc_path, const char *uri,
                                 const WeGlide::Pilot &pilot,
                                 uint_least32_t glider_id,
                                 UploadResponse &response,
                                 ProgressListener &progress) noexcept {
  boost::json::value json =
    co_await WeGlide::UploadFlight(*Net::curl, uri, pilot, glider_id,
                                   igc_path, progress);

  auto json_value = json.get_array().at(0);
  response.scoring_date = UTF8ToWideConverter(
      Json::Lookup(json_value, "scoring_date")->get_string().c_str());
  response.flight_id = Json::Lookup(json_value, "id")->get_int64();
  response.registration = UTF8ToWideConverter(
      Json::Lookup(json_value, "registration")->get_string().c_str());
  response.competition_id = UTF8ToWideConverter(
      Json::Lookup(json_value, "competition_id")->get_string().c_str());
  auto user = Json::Lookup(json_value, "user")->as_object();

  response.pilot.id =
      Json::Lookup(user, "id")->get_int64();
  response.pilot.name = UTF8ToWideConverter(
      Json::Lookup(user, "name")->get_string().c_str());

 auto aircraft = Json::Lookup(json_value, "aircraft")->as_object();

  response.glider.id = Json::Lookup(aircraft, "id")->get_int64();
  response.glider.name = UTF8ToWideConverter(Json::Lookup(aircraft, "name")->get_string().c_str());
  response.glider.kind = UTF8ToWideConverter(Json::Lookup(aircraft, "kind")->get_string().c_str());
  response.glider.sc_class = UTF8ToWideConverter(Json::Lookup(aircraft, "sc_class")->get_string().c_str());

}

bool UploadFlightDialog(Path igc_path, WeGlide::Pilot pilot, uint_least32_t glider_id) {
  const WeGlideSettings &weglide_settings =
      CommonInterface::GetComputerSettings().weglide;
  StaticString<0x1000> msg;
  NarrowString<0x200> url(weglide_settings.default_url);
  url += "/igcfile";

  try {
    if (glider_id == 0)
      glider_id = CommonInterface::GetComputerSettings().plane
        .weglide_glider_type;
    if (pilot.id == 0) {
      pilot = weglide_settings.pilot;
    }
    UploadResponse response;
  PluggableOperationEnvironment env;
  LogFormat(_("WeGlide Upload: %s"), igc_path.c_str());
//  if (!igc_path.exists()) {
//  Without file.exists rturn witch error...!!!
//   Path has no exists function???
//  }

  if (ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
                   _("Download"), UpdateTask(igc_path, url, pilot,
                              glider_id, response, env), &env) == false)
  {
     return false;
  }
    // UpdateList();
  msg.Format(_("File upload '%s' was succesfull!\n\n"), igc_path.c_str());
  msg.AppendFormat(_("%-20s: %u\n"), _("flight_id"), response.flight_id);
  msg.AppendFormat(_("%-20s: %s\n"), _("scoring_date"),
                   response.scoring_date);
  msg.AppendFormat(_("%-20s: %s (%d)\n"), _("pilot"),
                   response.pilot.name, response.pilot.id);
  msg.AppendFormat(_("%-20s: %s (%u)\n"), _("aircraft type"),
                   response.glider.name, response.glider.id);
  msg.AppendFormat(_("%-20s: %s: %s "), _("glider"), _("reg"),
                   response.registration);
  msg.AppendFormat(_("%s: %s\n"), _("cid"),
                   response.competition_id);

  ShowMessageBox(
      msg,
      _T("WeGlide Upload"), MB_OK | MB_ICONEXCLAMATION);

  return true;
  } catch (const std::exception &e) {
      // Check for errors
    const char *curl_response[] = {
      "CURL failed: The requested URL returned error: 406",
      "CURL failed: The requested URL returned error: 400",
      "CURL failed: Failed to open/read local data from file/application"
    };

    if (StringIsEqual(e.what(), curl_response[0],  // 406 Not Acceptable
            strlen(curl_response[0]))) {
      msg.assign(_("http Error 406 Not Acceptable:\n"
                    "The IGC file probably already exists on the server!"
          "\nigc file: "));
      msg.append(igc_path.c_str());
    } else if (StringIsEqual(e.what(), curl_response[1], // 400 Bad Request
                             strlen(curl_response[1]))) {
      msg.assign(_("http Error 400 Bad Request:\n"
                    "Probably one of the parameter is wrong!"));
    } else if (StringIsEqual(e.what(), curl_response[2],  // Failed to...
                             strlen(curl_response[2]))) {
      msg.assign(_("Failed to open/read local data from file/application\n"));
      msg.append(igc_path.c_str());
    } else {
      char msg_str[0x200];
      std::snprintf(msg_str, sizeof(msg_str) - 1,
                    "\nURL: '%s' response error!\n%s\nigc file: ", url.c_str(),
                    e.what());
      msg = UTF8ToWideConverter(msg_str).c_str();
      msg.append(LocalPath(igc_path).c_str());
    }
    LogFormat(_("WeGlide Upload: %s"), msg.c_str());

    ShowMessageBox(msg, _T("WeGlide Upload"), MB_OK | MB_ICONEXCLAMATION);
    return false;
  }
}

} // namespace WeGlide
