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

#include "UploadFlight.hpp"
#include "WeGlideSettings.hpp"
#include "system/Path.hpp"
#include "Dialogs/CoDialog.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "json/ParserOutputStream.hxx"
#include "net/http/CoStreamRequest.hxx"
#include "net/http/Easy.hxx"
#include "net/http/Mime.hxx"
#include "net/http/Progress.hpp"
#include "net/http/Setup.hxx"
#include "net/http/Init.hpp"
#include "system/ConvertPathName.hpp"

#include <cinttypes>

namespace WeGlide {

static CurlMime
MakeUploadFlightMime(CURL *easy, const Pilot &pilot,
                     uint_least32_t glider_type,
                     Path igc_path)
{
  CurlMime mime{easy};
  mime.Add("file").Filename("igc_file").FileData(NarrowPathName{igc_path});

  char buffer[32];
  sprintf(buffer, "%u", pilot.id);
  mime.Add("user_id").Data(buffer);
  FormatISO8601(buffer, pilot.birthdate);
  mime.Add("date_of_birth").Data(buffer);
  sprintf(buffer, "%" PRIuLEAST32, glider_type);
  mime.Add("aircraft_id").Data(buffer);

  return mime;
}

Co::Task<boost::json::value>
UploadFlight(CurlGlobal& curl, const char *url,
             const Pilot &pilot,
             uint_least32_t glider_type,
             Path igc_path,
             ProgressListener &progress)
{
  CurlEasy easy{url};
  Curl::Setup(easy);
  const Net::ProgressAdapter progress_adapter{easy, progress};
  // easy.SetFailOnError disabled: HTTP errors are dealt with here at the end

  const auto mime = MakeUploadFlightMime(easy.Get(), pilot,
                                         glider_type, igc_path);
  easy.SetMimePost(mime.get());

  Json::ParserOutputStream parser;
  const auto response =
    co_await Curl::CoStreamRequest(curl, std::move(easy), parser);
  auto json =  parser.Finish();
  std::string error_msg = "";

  if (response.status < 400) {  // 4xx and 5xx are fail codes
    co_return json;  // return with the WeGlide upload json object
  } else {
    // a http error occurs, but not in curl - no SetFailOnError()
    switch (response.status) {
    case 400: error_msg = "HTTP failure code: 400 Bad Request:\n";
      error_msg += "Probably one of the parameter is wrong!";
      break;
    case 406: error_msg = "HTTP failure code: 406 Not Acceptable:\n";
      error_msg += "The IGC file probably already exists on the server!";
      break;
    default: error_msg = "HTTP failure code: "
      + std::to_string(response.status); break;
    }
    throw std::runtime_error(error_msg.c_str());
  }
}

} // namespace WeGlide
