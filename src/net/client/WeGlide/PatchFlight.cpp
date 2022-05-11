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

#include "PatchFlight.hpp"
#include "Settings.hpp"
#include "HttpResponse.hpp"
#include "lib/curl/CoStreamRequest.hxx"
#include "lib/curl/Easy.hxx"
#include "lib/curl/Mime.hxx"
#include "net/http/Progress.hpp"
#include "lib/curl/Setup.hxx"
#include "net/http/Init.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "json/ParserOutputStream.hxx"
#include "system/ConvertPathName.hpp"
#include "system/Path.hpp"
#include "util/StaticString.hxx"

#include <cinttypes>
#include <sstream>

#if 1
# define WEGLIDE_STAGING 1
#endif

namespace WeGlide {

//  static CurlMime
//  MakeUploadFlightMime(CURL *easy, const User &user,
//                       uint_least32_t flight_id)
//  {
//    CurlMime mime{easy};
// //   mime.Add("file").Filename("igc_file").FileData(NarrowPathName{igc_path});
//  
//    char buffer[32];
//  #if WEGLIDE_STAGING
//    #if 0
//    sprintf(buffer, "%u", user.id);
//    mime.Add("user_id").Data(buffer);
//    sprintf(buffer, "1970-01-01");
//    mime.Add("date_of_birth").Data(buffer);
//    #endif
//  #else  // WEGLIDE_STAGING
//    sprintf(buffer, "%u", user.id);
//    mime.Add("user_id").Data(buffer);
//    FormatISO8601(buffer, user.birthdate);
//    mime.Add("date_of_birth").Data(buffer);
//  #endif
// //   sprintf(buffer, "%" PRIuLEAST32, aircraft_id);
// //   mime.Add("aircraft_id").Data(buffer);
//    mime.Add("comment").Data("Das ist ein Test in XCSoar");
//  
//    return mime;
//  }

Co::Task<HttpResponse>
PatchFlight(CurlGlobal &curl, const User &user, uint_least32_t flight_id,
            const boost::json::value &patch, ProgressListener &progress)
{
  assert(!user.token.empty());

  NarrowString<0x200> url;
  url.Format("%s/flightdetail/%u", WeGlideSettings::default_url, flight_id);
  CurlEasy easy{url};
  Curl::Setup(easy);
  const Net::ProgressAdapter progress_adapter{easy, progress};
  // easy.SetFailOnError disabled: HTTP errors are dealt with here at the end

#if 0
  const char *text =
      "{ \"comment\": \"Das ist ein Test in XCSoar\", \"rescore\": false}";
  easy.SetRequestBody(text, strlen(text));
#else
  std::stringstream ss;
  ss << patch;
    //"{ \"comment\": \"Das ist ein Test in XCSoar\", \"rescore\": false}";
  char text[0x100];
  strcpy(text, ss.str().c_str());
  // easy.SetRequestBody(ss.str().c_str(), ss.str().length() + 1);
  easy.SetRequestBody(text, strlen(text));
#endif

  easy.PatchRequest();  // switch to 'PATCH' mode
  struct curl_slist *list = nullptr;
  list = curl_slist_append(list, "accept: application/json");
//  struct curl_slist *list = curl_slist_append(list, "accept: application/json");
  list = curl_slist_append(list, "Content-Type: application/json");
  // list = curl_slist_append(list, "xcsoar-token: 4basmmN1");
  list = curl_slist_append(list, user.GetTokenString().c_str());
  easy.SetRequestHeaders(list);
  Json::ParserOutputStream parser;
  const auto response =
      co_await Curl::CoStreamRequest(curl, std::move(easy), parser);

  if (list)
    curl_slist_free_all(list); /* free the list again */
  co_return HttpResponse({response.status, parser.Finish()});
}

} // namespace WeGlide
