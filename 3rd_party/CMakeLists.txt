if (ON)
  # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party # 3rd-Party 
  cmake_minimum_required(VERSION 3.15)
  
  message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")
  
  # set(EP_BASE "${THIRD_PARTY}/external")  # later remove 'External!' # not used up to now!
  set(EP_CMAKE "${THIRD_PARTY}/cmake")  # later remove 'cmake!'
  # set(EP_BINARIES "${BINARY_DIR}/3rd_party")  # Binaries dahin verlagern?
  option(USE_SYSTEM_${TARGET_CNAME} "Should we use the system ${TARGET_NAME}?" OFF)
  option(EP_BUILD_ALWAYS "Build 3rd party packages always?" OFF)
  option(EP_BUILD_IN_SOURCE "Build 3rd party in sources (Recommended: OFF)?" OFF)

file(GLOB_RECURSE SCRIPT_FILES *.txt *.cmake *.in)

  include(boost.cmake)
  include(zlib.cmake)
  include(lua.cmake)
  include(jasper.cmake)
  include(zzip.cmake)
  include(mapserver.cmake)
  include(libpng.cmake)
  include(curl.cmake)
  include(xmlparser.cmake)
  include(c-ares.cmake)
  # branch cmake   include(googletest.cmake)
  # include(Qt5.cmake)
  # include(googlmock.cmake)
  # noch nicht! include(boost.cmake)
  set(THIRDPARTY_INCLUDES ${THIRDPARTY_INCLUDES} PARENT_SCOPE)



source_group("Scripts" FILES  ${SCRIPT_FILES})

add_custom_target(ThirdParty
    # EXCLUDE_FROM_ALL
    SOURCES
    ${SCRIPT_FILES}
)

endif()
