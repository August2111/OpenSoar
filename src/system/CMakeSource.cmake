set(system_SOURCES
        system/EventPipe.cxx
        system/FileMapping.cpp
        system/FileUtil.cpp
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

