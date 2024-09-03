set(_SOURCES
        Input/InputConfig.cpp
        Input/InputDefaults.cpp
        Input/InputEvents.cpp
        Input/InputEventsActions.cpp
        Input/InputEventsAirspace.cpp
        Input/InputEventsDevice.cpp
        Input/InputEventsLua.cpp
        Input/InputEventsMap.cpp
        Input/InputEventsPage.cpp
        Input/InputEventsSettings.cpp
        Input/InputEventsTask.cpp
        Input/InputEventsThermalAssistant.cpp
        Input/InputEventsTraffic.cpp
        Input/InputEventsVega.cpp
        Input/InputKeys.cpp
        Input/InputLookup.cpp
        Input/InputParser.cpp
        Input/InputQueue.cpp
        Input/TaskEventObserver.cpp
)

if (TARGET_IS_OPENVARIO)
  set(DEFAULT_XCI_FILE   defaultOV.xci)
else()
  set(DEFAULT_XCI_FILE   default.xci)
endif()

set(SCRIPT_FILES
    ../../Data/Input/${DEFAULT_XCI_FILE}
    CMakeSource.cmake
)
