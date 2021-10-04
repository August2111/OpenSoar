set(DISPLAY_STRING "# CURL           # CURL           # CURL           # CURL           # CURL")
message(STATUS "${DISPLAY_STRING}")
cmake_minimum_required(VERSION 3.15)

set(LIB_TARGET_NAME                                       curl)
#==========================================================
string(TOUPPER ${LIB_TARGET_NAME} TARGET_CNAME)

set(ZLIB_DIR ${LINK_LIBS}/zlib/${XCSOAR_ZLIB_VERSION})

# ---------------------------------------------------------------------------
### option(USE_SYSTEM_${TARGET_CNAME} "Should we use the system ${LIB_TARGET_NAME}?" OFF)
### option(USE_DRAHEIM "Should we use the draheim (or the BBDE) system?" OFF)


  string(REPLACE "curl-" "" CURL_VERSION ${XCSOAR_CURL_VERSION})  # defined in 3rd_Party.cmake
 # set(${TARGET_CNAME}_VERSION "7.78.0")
  string(REPLACE "." "_" GIT_TAG ${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION})
   # message(FATAL_ERROR "### GIT_TAG = ${GIT_TAG}")
  set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
  set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")


set(INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
# f.e. "on Windows: D:/Project/link_libs/curl/crl-7.69.1""
#-------------------
if(OFF)  # NOT EXISTS "${INSTALL_DIR}")
  ExternalProject_Add(
     ${LIB_TARGET_NAME}
   GIT_REPOSITORY "https://github.com/curl/curl.git"
   GIT_TAG  ${GIT_TAG}
     PREFIX  "${${TARGET_CNAME}_PREFIX}"
     BINARY_DIR    "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
     INSTALL_DIR "${INSTALL_DIR}"  # ${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}"
     # PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/CURL_CMakeLists.txt.in" <SOURCE_DIR>/CMakeLists.txt
     CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
         "-DCMAKE_INSTALL_BINDIR=bin/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_LIBDIR=lib/${TOOLCHAIN}"
         # "-DCMAKE_INSTALL_COMPONENT=bin/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_INCLUDEDIR=include"
         "-DBUILD_CURL_EXE=OFF"
         "-DBUILD_SHARED_LIBS=OFF"
         "-DBUILD_TESTING=OFF"
         "-DCMAKE_USE_LIBSSH2=OFF"
         "-DUSE_WIN32_LDAP=OFF"
         "-DZLIB_INCLUDE_DIR=${ZLIB_DIR}/include"
         "-DZLIB_LIBRARY_DEBUG=${ZLIB_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}zlibstaticd.${LIB_EXTENSION}"
         "-DZLIB_LIBRARY_RELEASE=${ZLIB_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}zlibstatic.${LIB_EXTENSION}"
    # BUILD_ALWAYS ${EP_BUILD_ALWAYS}
    BUILD_ALWAYS OFF
    BUILD_IN_SOURCE ${EP_BUILD_IN_SOURCE}
    DEPENDS  zlib
)
endif()
# list(APPEND THIRDPARTY_INCLUDES "${LIB_TARGET_NAME}" PARENT_SCOPE)
if (MSVC)  # unfortunately the lib name is a little bit 'tricky' at curl..
  set(LIB_NAME lib${LIB_TARGET_NAME})
else()
  set(LIB_NAME ${PRE_LIB}${LIB_TARGET_NAME})
endif()

# TODO(aug): move this to a macro!!!
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


# TOOLCHAIN=msvc2019
#      CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
#          "-DCMAKE_INSTALL_BINDIR=bin/%TOOLCHAIN%"
#          "-DCMAKE_INSTALL_LIBDIR=lib/%TOOLCHAIN}"
#          "-DCMAKE_INSTALL_INCLUDEDIR=include"
#          "-DBUILD_CURL_EXE=OFF"
#          "-DBUILD_SHARED_LIBS=OFF"
#          "-DBUILD_TESTING=OFF"
#          "-DCMAKE_USE_LIBSSH2=OFF"
#          "-DUSE_WIN32_LDAP=OFF"
#          "-DZLIB_INCLUDE_DIR=${ZLIB_DIR}/include"
#          "-DZLIB_LIBRARY_DEBUG=${ZLIB_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}zlibstaticd.${LIB_EXTENSION}"
#          "-DZLIB_LIBRARY_RELEASE=${ZLIB_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}zlibstatic.${LIB_EXTENSION}"

# D:\Projects\link_libs\openssl\OpenSSL-Win64\lib\VC\static\libssl64MDd.lib
# D:\Projects\link_libs\openssl\OpenSSL-Win64\lib\VC\static\libcrypto64MDd.lib
# Crypt32.lib
# CryptNet.lib
# cryptdll.lib