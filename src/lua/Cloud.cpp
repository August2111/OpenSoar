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

#include "Cloud.hpp"
#include "Util.hxx"
#include "Interface.hpp"
#include "LocalPath.hpp"
#include "system/Path.hpp"
#include "net/client/WeGlide/UploadIGCFile.hpp"
#include "net/client/WeGlide/DownloadTask.hpp"
#include "util/ConvertString.hpp"

extern "C" {
#include <lauxlib.h>
}

static int
l_cloud_index([[maybe_unused]] lua_State *L)
{
  return 1;
}


static int
l_igcfile_upload(lua_State *L)
{
  if (lua_gettop(L) == 0 || lua_gettop(L) > 5)
    return luaL_error(L, "Invalid parameters");
  if (StringIsEqualIgnoreCase(luaL_checkstring(L, 1), "weglide")) {
    Path igc_filepath(UTF8ToWideConverter(luaL_checkstring(L, 2)));
    if (!igc_filepath.IsAbsolute())
      igc_filepath = LocalPath(igc_filepath);

    // User ID:
    WeGlide::User user({static_cast<uint32_t>(
        lua_gettop(L) > 2 ? luaL_checknumber(L, 3) : 0)});
    // User Date of Birth:
    if (lua_gettop(L) > 3) {
#if 0
      unsigned day = 0, month = 0, year = 0;
      // Date format should be ISO8601 (2000-01-01)
      if (sscanf(luaL_checkstring(L, 4), "%04u-%02u-%02u", &year, &month,
                 &day) == 3) { // sscanf 3 parameter
        user.birthdate = {year, month, day};
      }
#else
      user.token = UTF8ToWideConverter(luaL_checkstring(L, 4)).c_str();
#endif
    }
    // Aircraft:
    auto aircraft =
        WeGlide::Aircraft({(uint32_t)(lua_gettop(L) > 4 ?
                          luaL_checknumber(L, 5) : 0)});
    if (WeGlide::UploadIGCFile(igc_filepath, user, aircraft)) {
        //success!
    }
  } else {
    return luaL_error(L, "Invalid parameter!");
  }
  return 0;
}

static int
l_task_download(lua_State *L)
{
  if (lua_gettop(L) > 1)
    return luaL_error(L, "Invalid parameters");
    
  WeGlide::User user({
      static_cast<uint32_t>(lua_gettop(L) > 0 ? luaL_checknumber(L, 1) : 0)});

  WeGlide::DownloadTaskFile(user);
  return 0;
}

static constexpr struct luaL_Reg settings_funcs[] = {
  {"task_download", l_task_download},
  {"igcfile_upload", l_igcfile_upload},
  {nullptr, nullptr}
};

void
Lua::InitCloud(lua_State *L)
{
  lua_getglobal(L, PROGRAM_NAME_LC );
  lua_newtable(L);
  lua_newtable(L);
  SetField(L, -2, "__index", l_cloud_index);
  lua_setmetatable(L, -2);

  luaL_setfuncs(L, settings_funcs, 0);
  lua_setfield(L, -2, "cloud");
  lua_pop(L, 1);
}
