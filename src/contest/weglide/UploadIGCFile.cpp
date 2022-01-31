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
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "LogFile.hpp"
#include "Cloud/weglide/UploadFlight.hpp"
#include "Cloud/weglide/WeGlideSettings.hpp"
#include "co/InvokeTask.hxx"
#include "Dialogs/Message.hpp"
#include "Dialogs/Contest/WeGlide/FlightDataDialog.hpp"
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

#include <cinttypes>

// Wrapper for getting converted string values of a json string
static const UTF8ToWideConverter 
GetJsonString(boost::json::standalone::value json_value, std::string_view key)
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

struct CoInstance {
  boost::json::value value;
  Co::InvokeTask
UpdateTask(Path igc_path, const User &user,
    uint_least32_t glider_id, ProgressListener &progress)
  {
    value = co_await UploadFlight(*Net::curl, user, glider_id,
      igc_path, progress);
  }
};

static Flight
UploadFile(Path igc_path, User user, uint_least32_t glider_id,
           StaticString<0x1000> &msg) noexcept
{
  try {
    if (glider_id == 0)
      glider_id =
          CommonInterface::GetComputerSettings().plane.weglide_glider_type;
    if (user.id == 0) {
      user = CommonInterface::GetComputerSettings().weglide.user;
    }

    PluggableOperationEnvironment env;
    CoInstance instance;
    if (!File::Exists(igc_path)) {
      msg.Format(_("'%s' - %s"), igc_path.c_str(), _("File doesn't exist"));
    } else if (ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
                     _("Upload Flight"),
                     instance.UpdateTask(igc_path, user, glider_id, env),
                     &env) == false) {
      msg.Format(_("'%s' - %s"), igc_path.c_str(),
                 _("ShowCoDialog with failure"));
    } else {
      // read the important data from json in a structure
      Flight flight_data(UploadJsonInterpreter(instance.value));
      flight_data.igc_name = igc_path.GetBase().c_str();

      msg.Format(_("File upload '%s' was successful"),
                 flight_data.igc_name.c_str()); // igc_path.c_str());
      return flight_data;                       // upload successful!
    }
  }
  catch (const std::exception &e) {
    msg.Format(_("'%s' - %s"), igc_path.c_str(),
      UTF8ToWideConverter(e.what()).c_str());
  }
  return Flight({0});  // upload failed!
}

bool
UploadIGCFile(Path igc_path, const User &user,
              uint_least32_t glider_id) noexcept
{ 
  try {
    StaticString<0x1000> msg;
    auto flight_data = UploadFile(igc_path, user, glider_id, msg);
    if (flight_data.flight_id > 0) {
      // upload successful!
      FlightDataDialog(flight_data, msg.c_str());
      return true;
    } else {
      // upload failed!
      LogFormat(_T("%s: %s!"), _("WeGlide Upload Error"), msg.c_str());
      ShowMessageBox(msg.c_str(), _("WeGlide Upload Error"),
        MB_ICONEXCLAMATION);
    }
  } catch (...) {
    LogError(std::current_exception());
  }
  return false;
}

bool
UploadIGCFile(Path igc_path) noexcept {
  return UploadIGCFile(igc_path, { 0 }, 0);
}

} // namespace WeGlide
