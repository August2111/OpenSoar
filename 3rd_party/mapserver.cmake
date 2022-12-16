if(0)
set(DISPLAY_STRING "# MAPSERVER      # MAPSERVER      # MAPSERVER      # MAPSERVER      # MAPSERVER")
message(STATUS "${DISPLAY_STRING}")
cmake_minimum_required(VERSION 3.15)

set(LIB_TARGET_NAME                                  mapserver)
#==========================================================
string(TOUPPER ${LIB_TARGET_NAME} TARGET_CNAME)

  set(${TARGET_CNAME}_VERSION "xcsoar")  # gdraheim

  set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
  set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_FILE "${THIRD_PARTY}/mapserver/mapserver-xcsoar.zip")
  if (UNIX)  # only temporarily
      set(${TARGET_CNAME}_FILE "/home/august/Projects/Gliding/Download/mapserver-xcsoar.7z")
  endif()
#  if(EXISTS      "${${TARGET_CNAME}_FILE}")
     set(${TARGET_CNAME}_URL "file://${${TARGET_CNAME}_FILE}")
  if(ON)
     set(${TARGET_CNAME}_URL "http://www.FlapsOnline.de/XCSoarAug/mapserver-xcsoar.zip")
#  if (EXIST      "${${TARGET_CNAME}_URL}")
  # ------------------
set(INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
#-------------------
# if(NOT EXISTS "${INSTALL_DIR}")
if(${WITH_3RD_PARTY})
if (MSVC OR MINGW)
  set( BINARY_STEP BINARY_DIR "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}")
else()
  set(BINARY_STEP   )
endif()
    ExternalProject_Add(
       ${LIB_TARGET_NAME}
       URL "${${TARGET_CNAME}_URL}"
       PREFIX  "${${TARGET_CNAME}_PREFIX}"
       # BINARY_DIR    "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
       INSTALL_DIR "${INSTALL_DIR}"      # "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}"
       CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
         "-DCMAKE_INSTALL_LIBDIR=lib/${TOOLCHAIN}"  # :PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_INCLUDEDIR=include"  #  :PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
         "-DXCSOAR_DIR=${PROJECTGROUP_SOURCE_DIR}/src"
         "-DLINK_LIBS=${LINK_LIBS}"
         "-DZZIP_INCLUDE_DIR=${LINK_LIBS}/zzip/${XCSOAR_ZZIP_VERSION}/include"
         "-DXCSOAR_ZZIP_VERSION=${XCSOAR_ZZIP_VERSION}"
        INSTALL_COMMAND   cmake --build . --target install --config Release
        BUILD_ALWAYS ${EP_BUILD_ALWAYS}
        BUILD_ALWAYS OFF
        DEPENDS zlib # zzip
    )
    endif()
  else()
    message(STATUS "!!! ZZIP-XCSOAR DON'T EXISTS !!!!!!!!!!!!!!!!!!!!!!!")
  endif()
set(${TARGET_CNAME}_LIB  "${INSTALL_DIR}/lib/${TOOLCHAIN}/${LIB_PREFIX}${LIB_TARGET_NAME}${LIB_SUFFIX}")
set(${TARGET_CNAME}_INCLUDE_DIR  "${INSTALL_DIR}/include")
# PARENT_SCOPE only available in Parent, not here...
if(EXISTS "${${TARGET_CNAME}_LIB}")
  set(${TARGET_CNAME}_LIB  ${${TARGET_CNAME}_LIB} PARENT_SCOPE)
else()
  set(${TARGET_CNAME}_LIB  ${LIB_TARGET_NAME} PARENT_SCOPE)
endif()
set(${TARGET_CNAME}_INCLUDE_DIR  ${${TARGET_CNAME}_INCLUDE_DIR} PARENT_SCOPE)

list(APPEND THIRDPARTY_INCLUDES ${${TARGET_CNAME}_INCLUDE_DIR})

endif(0)