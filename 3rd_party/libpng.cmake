set(DISPLAY_STRING "# LIBPNG         # LIBPNG         # LIBPNG         # LIBPNG         # LIBPNG")
message(STATUS "${DISPLAY_STRING}")
cmake_minimum_required(VERSION 3.15)
# get_filename_component(LIB_TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
set(LIB_TARGET_NAME                                 libpng)
#===================================================
string(TOUPPER ${LIB_TARGET_NAME} TARGET_CNAME)

# ---------------------------------------------------------------------------
option(USE_SYSTEM_${TARGET_CNAME} "Should we use the system ${LIB_TARGET_NAME}?" OFF)

# set(${TARGET_CNAME}_VERSION "1.5.10") # https://github.com/LuaDist/libpng
# set(${TARGET_CNAME}_VERSION "1.6.34") # https://github.com/winlibs/libpng
set(${TARGET_CNAME}_VERSION "1.6.37") # https://github.com/glennrp/libpng.git
set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")


set(ZLIB_DIR ${LINK_LIBS}/zlib/${XCSOAR_ZLIB_VERSION})

# message(FATAL_ERROR "### ${TARGET_CNAME}_PREFIX = ${${TARGET_CNAME}_PREFIX}")
#-------------------
set(INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
#-------------------
if(NOT EXISTS "${INSTALL_DIR}")
    ExternalProject_Add(
       ${LIB_TARGET_NAME}
       # GIT_REPOSITORY "https://github.com/winlibs/${LIB_TARGET_NAME}.git"
       # GIT_REPOSITORY "https://github.com/LuaDist/${LIB_TARGET_NAME}.git"
       GIT_REPOSITORY "https://github.com/glennrp/${LIB_TARGET_NAME}.git"
       GIT_TAG "v${${TARGET_CNAME}_VERSION}"           # git tag by jasper!
    
       PREFIX  "${${TARGET_CNAME}_PREFIX}"
       BINARY_DIR  "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
       INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}"
    
    #   PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/LIBPNG/CMakeLists.txt.in" <SOURCE_DIR>/CMakeLists.txt
       CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
           "-DCMAKE_INSTALL_LIBDIR=lib/${TOOLCHAIN}"
           "-DCMAKE_INSTALL_INCLUDEDIR=include"

             "-DZLIB_INCLUDE_DIR=${ZLIB_DIR}/include"
             "-DZLIB_LIBRARY_DEBUG=${ZLIB_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}zlibstaticd.${LIB_EXTENSION}"
             "-DZLIB_LIBRARY_RELEASE=${ZLIB_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}zlibstatic.${LIB_EXTENSION}"

             "-DPNG_SHARED=OFF"
             "-DPNG_STATIC=ON"
             "-DPNG_TEST=OFF"
        # BUILD_ALWAYS ${EP_BUILD_ALWAYS}
        BUILD_ALWAYS OFF
       BUILD_IN_SOURCE ${EP_BUILD_IN_SOURCE}
       DEPENDS zlib
    )
endif()

if (MSVC)  # unfortunately the lib name is a little bit 'tricky' at libPng..
  set(LIB_NAME libpng16_static)
else()
  set(LIB_NAME libpng16)
endif()

set(${TARGET_CNAME}_LIB  "${INSTALL_DIR}/lib/${TOOLCHAIN}/${LIB_NAME}.${LIB_EXTENSION}")

set(${TARGET_CNAME}_INCLUDE_DIR  "${INSTALL_DIR}/include")
# PARENT_SCOPE only available in Parent, not here...
if(EXISTS "${${TARGET_CNAME}_LIB}")
  set(${TARGET_CNAME}_LIB  ${${TARGET_CNAME}_LIB} PARENT_SCOPE)
else()
  set(${TARGET_CNAME}_LIB  ${LIB_TARGET_NAME} PARENT_SCOPE)
endif()
set(${TARGET_CNAME}_INCLUDE_DIR  ${${TARGET_CNAME}_INCLUDE_DIR} PARENT_SCOPE)

set(THIRDPARTY_INCLUDES ${THIRDPARTY_INCLUDES} ${${TARGET_CNAME}_INCLUDE_DIR})

