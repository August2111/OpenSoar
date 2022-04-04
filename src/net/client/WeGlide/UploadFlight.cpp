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
#include "Settings.hpp"
#include "HttpResponse.hpp"
#include "net/http/Progress.hpp"
#include "lib/curl/CoStreamRequest.hxx"
#include "lib/curl/Easy.hxx"
#include "lib/curl/Mime.hxx"
#include "lib/curl/Setup.hxx"
#include "net/http/Init.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "json/ParserOutputStream.hxx"
#include "system/ConvertPathName.hpp"
#include "system/Path.hpp"
#include "util/StaticString.hxx"

#include <cinttypes>


#if 1
# define WEGLIDE_STAGING 0
#endif

namespace WeGlide {

static CurlMime
MakeUploadFlightMime(CURL *easy, const User &user,
                     const Aircraft &aircraft,
                     Path igc_path)
{
  CurlMime mime{easy};
  mime.Add("file").Filename("igc_file").FileData(NarrowPathName{igc_path});

  char buffer[32];
  if (user.token.empty()) {
#if WEGLIDE_STAGING
  sprintf(buffer, "%u", user.id);
  mime.Add("user_id").Data(buffer);
  sprintf(buffer, "1970-01-01");
  mime.Add("date_of_birth").Data(buffer);
#else  // WEGLIDE_STAGING
  sprintf(buffer, "%u", user.id);
  mime.Add("user_id").Data(buffer);
  FormatISO8601(buffer, user.birthdate);
#endif
  mime.Add("date_of_birth").Data(buffer);
  sprintf(buffer, "%" PRIuLEAST32, aircraft.id);
  }
  mime.Add("aircraft_id").Data(buffer);

  return mime;
}

Co::Task<HttpResponse>
UploadFlight(CurlGlobal &curl,
             const User &user,
             const Aircraft &aircraft,
             Path igc_path,
             ProgressListener &progress)
{
  NarrowString<0x200> url;
  url.Format("%s/igcfile", WeGlideSettings::default_url);
  CurlEasy easy{url};
  Curl::Setup(easy);
  const Net::ProgressAdapter progress_adapter{easy, progress};
  // easy.SetFailOnError disabled: HTTP errors are dealt with here at the end

  const auto mime = MakeUploadFlightMime(easy.Get(), user,
                                         aircraft, igc_path);
  easy.SetMimePost(mime.get());


  struct curl_slist *list = nullptr;
  if (!user.token.empty()) {
    list = curl_slist_append(list, user.GetTokenString().c_str());
    easy.SetRequestHeaders(list);
  }
  Json::ParserOutputStream parser;
  const auto response =
    co_await Curl::CoStreamRequest(curl, std::move(easy), parser);
#if 1
  if (list)
    curl_slist_free_all(list); /* free the list */
#endif
  co_return HttpResponse({response.status, parser.Finish()});
}

} // namespace WeGlide
