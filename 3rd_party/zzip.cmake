set(DISPLAY_STRING "# ZLIB           # ZLIB           # ZLIB           # ZLIB           # ZLIB ")
message(STATUS "${DISPLAY_STRING}")
cmake_minimum_required(VERSION 3.15)

set(LIB_TARGET_NAME                                       zzip)
#==========================================================
string(TOUPPER ${LIB_TARGET_NAME} TARGET_CNAME)

set(ZLIB_DIR ${LINK_LIBS}/zlib/${XCSOAR_ZLIB_VERSION})

# ---------------------------------------------------------------------------
option(USE_SYSTEM_${TARGET_CNAME} "Should we use the system ${LIB_TARGET_NAME}?" OFF)
option(USE_DRAHEIM "Should we use the draheim (or the BBDE) system?" OFF)

set(INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
#-------------------
if(NOT EXISTS "${INSTALL_DIR}")

if (OFF)   # USE_DRAHEIM)
  # Diese github-Version
  set(${TARGET_CNAME}_VERSION "0.13.70")  # gdraheim

  set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
  set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(CONFIGURE_COMMAND "PATH=%PATH%;D:\\Programs\\msys\\msys1.0.11\\bin")
  # ------------------
    ExternalProject_Add(
       ${LIB_TARGET_NAME}
       GIT_REPOSITORY "https://github.com/gdraheim/zziplib.git"
       GIT_TAG "v${${TARGET_CNAME}_VERSION}"
    
       PREFIX  "${${TARGET_CNAME}_PREFIX}"
       BINARY_DIR    "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
       INSTALL_DIR "${INSTALL_DIR}"
       CONFIGURE_COMMAND "${CONFIGURE_COMMAND}"
    
       CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
       "-DINSTALL_BIN_DIR:PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
       "-DINSTALL_LIB_DIR:PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
       "-DZZIP_PACKAGE_NAME=\"${TARGET_CNAME}\""
       "-DZZIP_PACKAGE_VERSION=\"${TARGET_CNAME}_VERSION\""
       # BUILD_ALWAYS ${EP_BUILD_ALWAYS}
       BUILD_ALWAYS OFF
       BUILD_IN_SOURCE OFF
    )
#-------------------
elseif(USE_DDEBIN)
  set(${TARGET_CNAME}_VERSION "0.36c")    # ddebin
  
  set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
  set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  ExternalProject_Add(
     ${LIB_TARGET_NAME}
     GIT_REPOSITORY "https://github.com/ddebin/zzip.git"
     GIT_TAG "${${TARGET_CNAME}_VERSION}"
  
     PREFIX  "${${TARGET_CNAME}_PREFIX}"
     BINARY_DIR    "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
       INSTALL_DIR "${INSTALL_DIR}"
     PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/zzip_CMakeLists.txt.in" <SOURCE_DIR>/CMakeLists.txt
     CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
         "-DCMAKE_INSTALL_BINDIR=bin/${TOOLCHAIN}"  #  :PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_LIBDIR=lib/${TOOLCHAIN}"  # :PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_COMPONENT=bin/${TOOLCHAIN}"  # :PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_INCLUDEDIR=include"  #  :PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
         "-DZLIB_INCLUDEDIR=${LINK_LIBS}/zlib/${XCSOAR_ZLIB_VERSION}/include"
         "-DZLIB_LIBRARY_DEBUG=${LINK_LIBS}/zlib/${XCSOAR_ZLIB_VERSION}/lib/${TOOLCHAIN}/zlibstatic.lib"
         "-DZLIB_LIBRARY_RELEASE=${LINK_LIBS}/zlib/${XCSOAR_ZLIB_VERSION}/lib/${TOOLCHAIN}/zlibstatic.lib"
     BUILD_ALWAYS ON
     BUILD_IN_SOURCE OFF
  )
else()
  #  if (EXIST      "file://D:/Projects/3rd_Party/zzip/zzip-xcsoar.zip")

  set(${TARGET_CNAME}_VERSION "xcsoar")    # ddebin
  
  set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
  set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
  set(${TARGET_CNAME}_FILE "${THIRD_PARTY}/zzip/zzip-xcsoar.zip")
#  if(EXISTS      "${${TARGET_CNAME}_FILE}")
#     set(${TARGET_CNAME}_URL "file://${${TARGET_CNAME}_FILE}")
  if(ON)
     set(${TARGET_CNAME}_URL "http://www.FlapsOnline.de/XCSoarAug/zzip-xcsoar.zip")
  ExternalProject_Add(
     ${LIB_TARGET_NAME}
     URL "${${TARGET_CNAME}_URL}"
     PREFIX  "${${TARGET_CNAME}_PREFIX}"
     BINARY_DIR    "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
       INSTALL_DIR "${INSTALL_DIR}"
     # PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_CNAME}_CMakeLists.txt.in" <SOURCE_DIR>/CMakeLists.txt
     CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
         "-DCMAKE_INSTALL_BINDIR=bin/${TOOLCHAIN}"  #  :PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_LIBDIR=lib/${TOOLCHAIN}"  # :PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_COMPONENT=bin/${TOOLCHAIN}"  # :PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
         "-DCMAKE_INSTALL_INCLUDEDIR=include"  #  :PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"

         "-DZLIB_INCLUDE_DIR=${ZLIB_DIR}/include"
         "-DZLIB_LIBRARY_DEBUG=${ZLIB_DIR}/lib/${TOOLCHAIN}/zlibstatic.lib"
         "-DZLIB_LIBRARY_RELEASE=${ZLIB_DIR}/lib/${TOOLCHAIN}/zlibstatic.lib"

    BUILD_ALWAYS ${EP_BUILD_ALWAYS}
    BUILD_IN_SOURCE ${EP_BUILD_IN_SOURCE}
    DEPENDS zlib
  )
  else()
    message(STATUS "!!! ZZIP-XCSOAR DON'T EXISTS !!!!!!!!!!!!!!!!!!!!!!!")
  endif()
endif()
endif()

# set(${TARGET_CNAME}_LIB  "${${TARGET_CNAME}_INSTALL_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}${LIB_TARGET_NAME}.${LIB_EXTENSION}" PARENT_SCOPE)
set(${TARGET_CNAME}_LIB  "${INSTALL_DIR}/lib/${TOOLCHAIN}/${PRE_LIB}${LIB_TARGET_NAME}.${LIB_EXTENSION}")
set(${TARGET_CNAME}_INCLUDE_DIR  "${INSTALL_DIR}/include")
# PARENT_SCOPE only available in Parent, not here...
if(EXISTS "${${TARGET_CNAME}_LIB}")
  set(${TARGET_CNAME}_LIB  ${${TARGET_CNAME}_LIB} PARENT_SCOPE)
else()
  set(${TARGET_CNAME}_LIB  ${LIB_TARGET_NAME} PARENT_SCOPE)
endif()
set(${TARGET_CNAME}_INCLUDE_DIR  ${${TARGET_CNAME}_INCLUDE_DIR} PARENT_SCOPE)

set(THIRDPARTY_INCLUDES ${THIRDPARTY_INCLUDES} ${${TARGET_CNAME}_INCLUDE_DIR})



