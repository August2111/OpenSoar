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

#ifdef VERSUCH_1

#if 1 // TODO(August2111)!!!!
// TODO(August2111): Check includes
#include "WeGlideSettings.hpp"
#include "lib/curl/Easy.hxx"
#include "lib/curl/Mime.hxx"
#include "lib/curl/Setup.hxx"
#include "net/http/CoStreamRequest.hxx"
#include "net/http/Init.hpp"
#include "net/http/Progress.hpp"
#include "system/ConvertPathName.hpp"
#include "system/Path.hpp"
#include "util/StaticString.hxx"
#include "json/ParserOutputStream.hxx"
#endif

namespace WeGlide {
#if 1 // TODO(August2111)!!!!

// struct WeGlideHttpResponse {
//   unsigned code;
//   boost::json::value json_value;
// };

Co::Task<WeGlideHttpResponse> DownloadTask(CurlGlobal &curl, const User &user,
                                           ProgressListener &progress) {
  NarrowString<0x200> url;
  url.Format("%s/task", WeGlideSettings::default_url);
  CurlEasy easy{url.c_str()};
  Curl::Setup(easy);
  const Net::ProgressAdapter progress_adapter{easy, progress};
  // easy.SetFailOnError disabled: HTTP errors are dealt with here at the end

  Json::ParserOutputStream parser;
  const auto response =
      co_await Curl::CoStreamRequest(curl, std::move(easy), parser);
  co_return WeGlideHttpResponse({response.status, parser.Finish()});
}
#endif

} // namespace WeGlide
//=================================================================================
#include "Dialogs/CoDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Interface.hpp"
#include "LogFile.hpp"
#include "Operation/PluggableOperationEnvironment.hpp"
#include "UIGlobals.hpp"
#include "co/InvokeTask.hxx"
#include "util/ConvertString.hpp"

#include "Cloud/weglide/WeGlideSettings.hpp"
#include "Dialogs/CoDialog.hpp"
#include "Dialogs/Contest/WeGlide/FlightUploadResponse.hpp"
#include "Dialogs/Error.hpp"
#include "Dialogs/Message.hpp"
#include "Formatter/TimeFormatter.hpp"
#include "Interface.hpp"
#include "Language/Language.hpp"
#include "LogFile.hpp"
#include "Operation/PluggableOperationEnvironment.hpp"
#include "UIGlobals.hpp"
// #include "UploadIGCFile.hpp"
#include "co/InvokeTask.hxx"
#include "net/http/Init.hpp"
#include "system/ConvertPathName.hpp"
#include "system/FileUtil.hpp"
#include "util/ConvertString.hpp"
#include "util/StaticString.hxx"
#include "json/ParserOutputStream.hxx"

#include <cinttypes>

namespace WeGlide {

struct CoInstance {
  WeGlideHttpResponse http = {0};
  Co::InvokeTask CoDownloadTask(const User &user, ProgressListener &progress) {
    http = co_await DownloadTask(*Net::curl, user, progress);
  }
};

static bool DownloadTask(User user, StaticString<0x1000> &msg) noexcept try {
  const auto settings = CommonInterface::GetComputerSettings();
  if (user.id == 0)
    user = settings.weglide.user;

  PluggableOperationEnvironment env;
  CoInstance instance;
  if (ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
                   _("Upload Flight"), instance.CoDownloadTask(user, env),
                   &env) == false) {
    msg.Format(_("'User %u' - %s"), user.id, _("ShowCoDialog with failure"));
  } else {
    switch (instance.http.code) {
    case 400:
      msg.Format(_T("%s: %u '%s'\n%s!"), _("HTTP failure code"),
                 instance.http.code, _("Bad Request"),
                 _("Probably one of the parameter is wrong"));
      break;
    case 406:
      msg.Format(_T("%s: %u '%s'\n%s!"), _("HTTP failure code"),
                 instance.http.code, _("Not Acceptable"),
                 _("The IGC file probably already exists on the server"));
      break;
    default:
      if (instance.http.code >= 200 && instance.http.code < 400 &&
          !instance.http.json_value.is_null()) {

        // read the important data from json in a structure
        // auto flight/task_data =
        // UploadJsonInterpreter(instance.http.json_value);
        return true; // upload successful!
      } else {
        msg.Format(_T("%s: %u"), _("HTTP failure code"), instance.http.code);
      }
      break;
    }
  }
  return false; // failure...
  // ???} catch (const std::exception &e) {
  // ???  msg.Format(_("'%s' - %s"), _T("Task"),
  // ???             UTF8ToWideConverter(e.what()).c_str());
  // ???  return false;
} catch (...) {
  msg.Format(_("'%s' - %s"), _("General Exception"), _T("Task"));
  // ???  ShowError(std::current_exception(), _T("WeGlide UploadFile"));
  return false;
}

bool
// DownloadOwnTask(uint_least32_t user_id)
DownloadTask(uint_least32_t user_id) {
  user_id = 511; // Test!!!!
  StaticString<0x1000> msg;
  User user(user_id);
  auto response = DownloadTask(user, msg);
  return true;
}

bool DownloadUserTask() {
  // Dialog!!!!
  return true;
}

} // namespace WeGlide

#endif // Versuch_1
//==========================================================================================================
//==========================================================================================================
//==========================================================================================================

#include "Dialogs/CoDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Interface.hpp"
#include "Language/Language.hpp"
#include "LocalPath.hpp"
#include "LogFile.hpp"
#include "Operation/PluggableOperationEnvironment.hpp"
#include "Settings.hpp"
#include "UIGlobals.hpp"
#include "co/InvokeTask.hxx"
#include "io/FileTransaction.hpp"
#include "net/http/CoDownloadToFile.hpp"
#include "net/http/Init.hpp"
#include "system/Path.hpp"

#include <cinttypes>

static Co::InvokeTask DownloadToFile(const char *url, Path path,
                                     ProgressListener &progress) {
  const auto ignored_response = co_await Net::CoDownloadToFile(
      *Net::curl, url, nullptr, nullptr, path, nullptr, progress);
}

namespace WeGlide {

AllocatedPath TaskToFile(User user) noexcept try {
  //  bool result = false;

  if (user.IsValid()) {
    NarrowString<0x100> url;
    //    const auto cache_path = MakeCacheDirectory(_T("weglide"));
    url.Format("%s/igcfile", WeGlideSettings::default_url);
    //    NarrowString<0x100> url(WeGlideSettings::default_url);
    //    url.AppendFormat("/task/declaration/%d?cup=false&tsk=true", user.id);
    url.AppendFormat("/task/declaration/%d", user.id);

    // Path path = cache_path;
    //  path += _T("/weglide.tsk");
    auto path = AllocatedPath::Build(MakeCacheDirectory(_T("weglide")),
                                     _T("weglide.tsk"));
    FileTransaction transaction(path);
    PluggableOperationEnvironment env;

    if (!ShowCoDialog(UIGlobals::GetMainWindow(), UIGlobals::GetDialogLook(),
                      _("Download WeGlide Task"),
                      DownloadToFile(url, transaction.GetTemporaryPath(), env),
                      &env))
      return AllocatedPath(); // -> dialog with failure!

    transaction.Commit();
    return path;
  } else {
    // user is invalid!
    return AllocatedPath();
  }
} catch (...) {
  // prevent a general exception in Download
  ShowError(std::current_exception(), _("WeGlide Task to File"));
  return AllocatedPath();
}

AllocatedPath DownloadTaskFile(User user) {
  if (!user.IsValid()) {
    // maybe select an other id!
    // TODO: Asking to load the Task with decision from which pilot!!!
    user = CommonInterface::GetComputerSettings()
               .weglide.pilot; // the preset value
  }

  return TaskToFile(user);
}

} // namespace WeGlide

//==========================================================================================================
//==========================================================================================================
//==========================================================================================================
