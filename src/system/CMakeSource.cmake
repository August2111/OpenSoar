set(system_SOURCES
        system/Clock.cpp
        # branch cmake moved to io system/FileDescriptor.cxx
        system/EventPipe.cxx
        system/FileMapping.cpp
        system/FileUtil.cpp
        # branch cmake gibt es nicht mehr ??? system/LogError.cpp
        system/Path.cpp
        system/PathName.cpp
        system/Process.cpp
        system/RunFile.cpp
        system/SystemLoad.cpp
)
if(UNIX)
  list(APPEND system_SOURCES
##        system/EventPipe.cpp
  )
endif()

