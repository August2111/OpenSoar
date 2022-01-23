set(lua_SOURCES
        lua/Airspace.cpp
        lua/Associate.cpp
        lua/Background.cpp
        lua/Basic.cpp
        lua/Blackboard.cpp
        lua/Catch.cpp
        # not yet!!! lua/Cloud.cpp
        lua/Dialogs.cpp
        lua/Error.cxx
        lua/Full.cpp
        lua/Geo.cpp
        lua/InputEvent.cpp
        lua/Legacy.cpp
        lua/Log.cpp
        lua/Http.cpp
        lua/Logger.cpp
        lua/Map.cpp
        lua/Persistent.cpp
        lua/Ptr.cpp
        lua/Replay.cpp
        lua/RunFile.cxx
        lua/Settings.cpp
        lua/StartFile.cpp
        lua/Task.cpp
        lua/Timer.cpp
        lua/Tracking.cpp
        lua/Wind.cpp
)
set(xcslua_SOURCES ${lua_SOURCES})

set(SCRIPT_FILES
    CMakeSource.cmake
    ${PROJECTGROUP_SOURCE_DIR}/doc/lua.rst
)
