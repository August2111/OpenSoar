set(DISPLAY_STRING "# ZLIB           # ZLIB           # ZLIB           # ZLIB           # ZLIB ")
message(STATUS "${DISPLAY_STRING}")
cmake_minimum_required(VERSION 3.15)

set(LIB_TARGET_NAME                                       zlib)
#==========================================================
string(TOUPPER ${LIB_TARGET_NAME} TARGET_CNAME)
# ---------------------------------------------------------------------------
option(USE_SYSTEM_${TARGET_CNAME} "Should we use the system ${LIB_TARGET_NAME}?" OFF)

# set(${TARGET_CNAME}_VERSION "1.2.11")
string(REPLACE "zlib-" "" ZLIB_VERSION ${XCSOAR_ZLIB_VERSION})

set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
#-------------------
set(INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
set(INSTALL_BIN "${INSTALL_DIR}/bin/${TOOLCHAIN}")
set(INSTALL_LIB "${INSTALL_DIR}/lib/${TOOLCHAIN}")
#----------------------
if(NOT EXISTS "${INSTALL_LIB}")
    ExternalProject_Add(
       ${LIB_TARGET_NAME}
       GIT_REPOSITORY "https://github.com/madler/zlib.git"
       GIT_TAG "v${${TARGET_CNAME}_VERSION}"
    
       PREFIX  "${${TARGET_CNAME}_PREFIX}"
       BINARY_DIR    "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
       INSTALL_DIR "${INSTALL_DIR}"     # "${LINK_LIBS}/zlib/${XCSOAR_${TARGET_CNAME}_VERSION}"
    
         CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
         "-DINSTALL_BIN_DIR:PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
         "-DINSTALL_LIB_DIR:PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
        # BUILD_ALWAYS ${EP_BUILD_ALWAYS}
        BUILD_ALWAYS OFF
        BUILD_IN_SOURCE ${EP_BUILD_IN_SOURCE}
    )
endif()

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
  set(LIB_NAME ${PRE_LIB}zlibstatic)
else()
  set(LIB_NAME ${PRE_LIB}z)
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

