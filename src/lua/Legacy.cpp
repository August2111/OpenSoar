// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Legacy.hpp"
#include "Util.hxx"
#include "Input/InputLookup.hpp"

extern "C" {
#include <lauxlib.h>
}

static int
l_fire_legacy_event(lua_State *L)
{
  const char *event = lua_tostring(L, 1);

  if (event == nullptr)
    return luaL_error(L, "No InputEvent specified");

  auto *event_function = InputEvents::findEvent(event);
  if (event_function == nullptr)
    return luaL_error(L, "Unknown InputEvent");

  const char *parameter = lua_tostring(L, 2);
  if (parameter != nullptr)
    event_function(parameter);
  return 0;
}

void
Lua::InitLegacy(lua_State *L)
{
  //  lua_getglobal(L, "xcsoar");
  lua_getglobal(L, PROGRAM_NAME_LC );
  SetField(L, RelativeStackIndex{-1},
           "fire_legacy_event", l_fire_legacy_event);
  lua_pop(L, 1);
}
