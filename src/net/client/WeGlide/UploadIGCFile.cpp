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
#include "Settings.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "LogFile.hpp"
#include "Cloud/weglide/UploadFlight.hpp"
#include "Cloud/weglide/WeGlideSettings.hpp"
#include "Cloud/weglide/HttpResponse.hpp"
#include "co/InvokeTask.hxx"
#include "Dialogs/Message.hpp"
#include "Dialogs/CoDialog.hpp"
#include "Dialogs/Error.hpp"
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

// Wrapper for getting converted string values of a json string
static const UTF8ToWideConverter 
GetJsonString(boost::json::value json_value, std::string_view key)
{
  return UTF8ToWideConverter(json_value.at(key).get_string().c_str());
}

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

// UploadSuccessDialog is only a preliminary DialogBox to show the 
// result of this upload
static void
UploadSuccessDialog(const Flight &flight_data, const TCHAR *msg)
{
  StaticString<0x1000> display_string;
  // TODO: Create a real Dialog with fields in 'src/Dialogs/Cloud/weglide'!
  // With this Dialog insert the possibilty to update/patch the flight
  // f.e. copilot in double seater, scoring class, short comment and so on
  display_string.Format(_T("%s\n\n%s: %u\n%s: %s\n%s: %s (%d)\n"
    "%s: %s (%u)\n%s: %s, %s: %s"), msg,
    _T("Flight ID"), flight_data.flight_id,
    _("Date"), flight_data.scoring_date.c_str(),
    _("Username"), flight_data.user.name.c_str(), flight_data.user.id,
    _("Plane"), flight_data.aircraft.name.c_str(), flight_data.aircraft.id,
    _("Registration"), flight_data.registration.c_str(),
    _("Comp. ID"), flight_data.competition_id.c_str());

  ShowMessageBox(display_string.c_str(), _("WeGlide Upload"), MB_OK);
}

struct CoInstance {
  HttpResponse http;
  Co::InvokeTask
  UpdateTask(Path igc_path, const User &user,
    uint_least32_t aircraft_id, ProgressListener &progress)
  {
    http = co_await UploadFlight(*Net::curl, user, aircraft_id,
      igc_path, progress);
  }
};

static Flight
UploadFile(Path igc_path, User user, uint_least32_t aircraft_id,
           StaticString<0x1000> &msg) noexcept
try {
  const auto settings = CommonInterface::GetComputerSettings();
  if (aircraft_id == 0)
    aircraft_id = settings.plane.weglide_glider_type;
  if (user.id == 0)
    user = settings.weglide.pilot;

  PluggableOperationEnvironment env;
  CoInstance instance;
  if (!File::Exists(igc_path)) {
    msg.Format(_("'%s' - %s"), igc_path.c_str(), _("File doesn't exist"));
  } else if (ShowCoDialog(UIGlobals::GetMainWindow(),
                          UIGlobals::GetDialogLook(), _("Upload Flight"),
                          instance.UpdateTask(igc_path, user, aircraft_id, env),
                          &env) == false) {
    msg.Format(_("'%s' - %s"), igc_path.c_str(),
               _("ShowCoDialog with failure"));
  } else if (instance.http.code >= 200 && instance.http.code < 400 &&
            !instance.http.json_value.is_null()) {

    // read the important data from json in a structure
    auto flight_data = UploadJsonInterpreter(instance.http.json_value);
    flight_data.igc_name = igc_path.GetBase().c_str();

    msg.Format(_("File upload '%s' was successful"), igc_path.c_str());
    return flight_data; // upload successful!
  } else {  // failure case
    msg.Format(_T("%s: %u"), _("HTTP failure code"), instance.http.code);
  }
  return Flight();  // failure...
} catch (const std::exception &e) {
  msg.Format(_T("'%s' - %s"), igc_path.c_str(),
    UTF8ToWideConverter(e.what()).c_str());
  return Flight();
}

bool
UploadIGCFile(Path igc_path, const User &user,
              uint_least32_t aircraft_id) noexcept
try {
  StaticString<0x1000> msg;
  auto flight_data = UploadFile(igc_path, user, aircraft_id, msg);
  if (flight_data.flight_id > 0) {
    // upload successful!
    LogFormat(_("%s: %s"), _("WeGlide Upload"), msg.c_str());
    UploadSuccessDialog(flight_data, msg.c_str());
    return true;
  } else {
    // upload failed!
    LogFormat(_T("%s: %s!"), _("WeGlide Upload Error"), msg.c_str());
    ShowMessageBox(msg.c_str(), _("WeGlide Upload Error"), MB_ICONEXCLAMATION);
  }
  return false;
} catch (...) {
  ShowError(std::current_exception(), _T("WeGlide UploadIGCFile"));
  return false;
}

} // namespace WeGlide
