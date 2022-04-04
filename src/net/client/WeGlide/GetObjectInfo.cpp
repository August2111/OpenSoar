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

#include "GetObjectInfo.hpp"
#include "Settings.hpp"
#include "HttpResponse.hpp"

#include "UIGlobals.hpp"
#include "GetJsonString.hpp"
#include "co/InvokeTask.hxx"
#include "co/Task.hxx"
#include "Dialogs/CoDialog.hpp"
#include "lib/curl/CoStreamRequest.hxx"
#include "lib/curl/Easy.hxx"
#include "net/http/Init.hpp"
#include "lib/curl/Setup.hxx"
#include "net/http/Progress.hpp"

#include "json/ParserOutputStream.hxx"
#include "Operation/PluggableOperationEnvironment.hpp"

static Co::Task<HttpResponse>
GetObjectInfoTask(CurlGlobal &curl, const char *url, ProgressListener &progress) {
  CurlEasy easy{url};
  Curl::Setup(easy);
  const Net::ProgressAdapter progress_adapter{easy, progress};
  // easy.SetFailOnError disabled: HTTP errors are dealt with here at the end
  
  Json::ParserOutputStream parser;

  const auto response =
      co_await Curl::CoStreamRequest(curl, std::move(easy), parser);
  co_return HttpResponse({response.status, parser.Finish()});
}

struct CoInstance {
  HttpResponse http;
  Co::InvokeTask Run(const char *url,
                            ProgressListener &progress) {
    http = co_await GetObjectInfoTask(*Net::curl, url,
                                 progress);
#ifdef _DEBUG  // TODO(August2111)
    std::string str = http.json_value.as_object().if_contains("name")->get_string().c_str();
    str = str;
#endif
  }
};

namespace WeGlide {

boost::json::object
GetJsonObject(const char *field, uint32_t object_id) noexcept
try {
  if (field) {
    // StaticString<0x400> msg;
    PluggableOperationEnvironment env;
    CoInstance instance;
    NarrowString<0x100> url;
    url.Format("%s/%s/%u", WeGlideSettings::default_url, field, object_id);
    if (ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
                     _T("Get WeGlide Object Info"), instance.Run(url, env),
                     &env) == true)
      if (instance.http.code >= 200 && instance.http.code < 400 &&
          !instance.http.json_value.is_null()) {
        return instance.http.json_value.as_object();
      }
  }
  return 0;
} catch (...) {
  return 0;
}

User
GetUserInfo(uint32_t user_id) noexcept
try {
  const auto wg_user = GetJsonObject("user", user_id);
  if (!wg_user.empty()) {
    User user({user_id});  // TODO(August2111): set user with id!?!
    user.name = GetJsonString(wg_user, "name");
#ifdef _DEBUG
    StaticString<0x40> str;
    str = GetJsonString(wg_user, "gender");
    auto home = wg_user.at("home_airport").as_object();
    str = GetJsonString(home, "name");
    auto club = wg_user.at("club").as_object();
    str = GetJsonString(club, "name");
    user.club = str;
    str = GetJsonString(club, "region");
#endif
    return user;
  }

  return User();
} catch (...) {
  return User();
}

Aircraft
GetAircraftInfo(uint32_t aircraft_id) noexcept
try {
  const auto wg_aircraft = GetJsonObject("aircraft", aircraft_id);
  if (!wg_aircraft.empty()) {
    Aircraft aircraft({aircraft_id});  // TODO(August2111): set Aircraft with id!?!
    aircraft.name = GetJsonString(wg_aircraft, "name");
    return aircraft;
  }

  return Aircraft();
} catch (...) {
  return Aircraft();
}

} // namespace WeGlide



