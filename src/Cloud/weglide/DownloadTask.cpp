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

#include "DownloadTask.hpp"

#if 0  // TODO(August2111)!!!!
#include "net/http/CoStreamRequest.hxx"
#include "net/http/Easy.hxx"
#include "net/http/Mime.hxx"
#include "net/http/Progress.hpp"
#include "net/http/Setup.hxx"
#include "Formatter/TimeFormatter.hpp"
#include "json/ParserOutputStream.hxx"
#endif

namespace WeGlide {
#if 0  // TODO(August2111)!!!!
    static Co::InvokeTask UpdateTask(const char *uri,
                                 const Pilot &pilot,
                                 UploadResponse &response,
                                 ProgressListener &progress) noexcept {
  boost::json::value json = co_await UploadFlight(
      *Net::curl, uri, pilot, glider_id, igc_path, progress);

  auto json_value = json.get_array().at(0);
  response.scoring_date = UTF8ToWideConverter(
      Json::Lookup(json_value, "scoring_date")->get_string().c_str());
  response.flight_id = Json::Lookup(json_value, "id")->get_int64();
  response.registration = UTF8ToWideConverter(
      Json::Lookup(json_value, "registration")->get_string().c_str());
  response.competition_id = UTF8ToWideConverter(
      Json::Lookup(json_value, "competition_id")->get_string().c_str());
  auto user = Json::Lookup(json_value, "user")->as_object();

  response.pilot.id = Json::Lookup(user, "id")->get_int64();
  response.pilot.name =
      UTF8ToWideConverter(Json::Lookup(user, "name")->get_string().c_str());

  auto aircraft = Json::Lookup(json_value, "aircraft")->as_object();

  response.glider.id = Json::Lookup(aircraft, "id")->get_int64();
  response.glider.name =
      UTF8ToWideConverter(Json::Lookup(aircraft, "name")->get_string().c_str());
  response.glider.kind =
      UTF8ToWideConverter(Json::Lookup(aircraft, "kind")->get_string().c_str());
  response.glider.sc_class = UTF8ToWideConverter(
      Json::Lookup(aircraft, "sc_class")->get_string().c_str());
}
#endif

bool DownloadTask(uint_least32_t pilot_id) {
  
  return true; 
}

} // namespace WeGlide
