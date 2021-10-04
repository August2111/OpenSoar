set(DISPLAY_STRING "# BOOST          # BOOST          # BOOST          # BOOST          # BOOST")
message(STATUS "${DISPLAY_STRING}")
cmake_minimum_required(VERSION 3.10)

# 24.09.2021
# set(BOOST_VERSION "1.76.0")
string(FIND ${XCSOAR_BOOST_VERSION} "-" BOOST_VERSION_START)  # defined in 3rd_Party.cmake
# string(REPLACE "boost-" "" BOOST_VERSION ${XCSOAR_BOOST_VERSION})  # defined in 3rd_Party.cmake
math(EXPR  BOOST_VERSION_START "${BOOST_VERSION_START}+1")
string(SUBSTRING ${XCSOAR_BOOST_VERSION} ${BOOST_VERSION_START}+2 100 BOOST_VERSION)  # defined in 3rd_Party.cmake
string(FIND ${BOOST_VERSION} "." BOOST_VERSION_LEN REVERSE )  # defined in 3rd_Party.cmake
string(SUBSTRING ${BOOST_VERSION} 0 ${BOOST_VERSION_LEN} BOOST_SHORTVERSION)  # defined in 3rd_Party.cmake

string(REPLACE "." "_" _BOOST_SHORTVERSION ${BOOST_SHORTVERSION})  # defined in 3rd_Party.cmake

# set(Boost_SHORTVERSION "1_76") 
# message(FATAL_ERROR "### BOOST_VERSION = ${BOOST_VERSION} ||| ${BOOST_VERSION_START} ||| ${BOOST_VERSION_LEN} ||| ${BOOST_SHORTVERSION} ||| ${_BOOST_SHORTVERSION} ")
set(BOOST_ROOT "${LINK_LIBS}/boost/boost-${BOOST_VERSION}") 
set(Boost_COMPILER "${TOOLCHAIN}") 
if (WIN32 AND MSVC)
      if(NOT Boost_COMPILER)
        set(Boost_COMPILER msvc2019)
      endif()
elseif(WIN32 AND MINGW)
      if(NOT Boost_COMPILER)
        set(Boost_COMPILER mgw11)
      endif()
elseif(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
      if(NOT Boost_COMPILER)
        set(Boost_COMPILER clang)
      endif()
elseif(WIN32 AND CLANG)
   message(FATAL_ERROR "+++ Unbekanntes System: CLANG!")
elseif(WIN32 AND NINJA)
   message(FATAL_ERROR "+++ Unbekanntes System: NINJA!")
elseif(UNIX)
      if($ENV{USER} STREQUAL "pcderad0633") 
         # why?
         set(Boost_COMPILER "gcc9") 
      else()
         set(Boost_COMPILER "gcc7")  # necessary: ??
      endif()
elseif(ANDROID)
    set(Boost_COMPILER "clang")  # necessary: changed clang8 as boost compiler to clang!
else()
   message(FATAL_ERROR "+++ Unbekanntes System: ${CMAKE_SYSTEM}!")
endif()


set(Boost_DIR "${BOOST_ROOT}/lib/${Boost_COMPILER}/cmake/Boost-${BOOST_VERSION}")
set(Boost_INCLUDE_DIR  "${BOOST_ROOT}/include/boost-${_BOOST_SHORTVERSION}")

if (NOT Boost_INCLUDE_DIR)
#   message(FATAL_ERROR "!!! Boost_INCLUDE_DIR is EMPTY")
#  if(EXISTS "${BOOST_ROOT}/include/boost-1_75/boost/version.hpp")
#       set(Boost_INCLUDE_DIR  "${BOOST_ROOT}/include/boost-1_77")
#       set(Boost_INCLUDE_DIR "/home/august/Projects/link_libs/boost/boost-1.77.0/include/boost-1_76")
#  elseif(EXISTS "${BOOST_ROOT}/include/boost/version.hpp")
#      set(Boost_INCLUDE_DIR "${BOOST_ROOT}/include")
#  else()
  if(NOT EXISTS "${Boost_INCLUDE_DIR}/boost/version.hpp")
      message(FATAL_ERROR "!!! Boost: include directories not found! (BOOST_ROOT = ${BOOST_ROOT})")
  endif()
endif()

if (NOT EXISTS ${Boost_INCLUDE_DIR}/boost/config.hpp)
    message(STATUS "!!! Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR} not correctly!")
    set(Boost_INCLUDE_DIR   ${Boost_INCLUDE_DIR}/boost-1_75)
    if (NOT EXISTS ${Boost_INCLUDE_DIR}/boost/config.hpp)
      message(FATAL_ERROR "!!! Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
    endif()
endif()
### message(FATAL_ERROR "BOOST-Stop: ${BOOST_ROOT} /// ${Boost_DIR}")
# 24.09.2021

set(LIB_TARGET_NAME                                       boost)
#==========================================================
string(TOUPPER ${LIB_TARGET_NAME} TARGET_CNAME)

# ---------------------------------------------------------------------------
option(USE_SYSTEM_${TARGET_CNAME} "Should we use the system ${LIB_TARGET_NAME}?" OFF)

## siehe oben:set(${TARGET_CNAME}_VERSION "1.77.0")
set(XCSOAR_${TARGET_CNAME}_VERSION "${LIB_TARGET_NAME}-${${TARGET_CNAME}_VERSION}")  # reset!
set(${TARGET_CNAME}_INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")
set(${TARGET_CNAME}_PREFIX "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")

if(MSVC)
    set(TOOLSET msvc)
    set(TOOLSETNAME msvc2019)
else()
    set(TOOLSET gcc)
    set(TOOLSETNAME ${TOOLCHAIN})
endif()


# set(${TARGET_CNAME}_BUILD_CMD "cd ../../src/${LIB_TARGET_NAME} & .\\b2 -j4 toolset=${TOOLSET} variant=release link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION} --build-dir=D:/Projects/3rd_Party/${LIB_TARGET_NAME}/build/${TOOLSETNAME} --with-chrono --with-system --with-filesystem --with-headers --with-date_time  --includedir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/include --libdir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/lib/${TOOLSETNAME} install")
# set(${TARGET_CNAME}_BUILD_CMD "cd ../../src/build & echo %CD% & ./b2 -j4 toolset=${TOOLSET} variant=release link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION} --build-dir=D:/Projects/3rd_Party/${LIB_TARGET_NAME}/build/${TOOLSETNAME} --with-chrono --with-system --with-filesystem --with-headers --with-date_time  --includedir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/include --libdir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/lib/${TOOLSETNAME} install")
# set(${TARGET_CNAME}_BUILD_CMD "cd ../../src/build & echo %CD% & ./b2 -j4 toolset=${TOOLSET} link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION} --build-dir=D:/Projects/3rd_Party/${LIB_TARGET_NAME}/build/${TOOLSETNAME} --with-chrono --with-system --with-filesystem --with-headers --with-date_time  --includedir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/include --libdir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/lib/${TOOLSETNAME} install")
set(${TARGET_CNAME}_BUILD_CMD "./b2 -j4 toolset=${TOOLSET} link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION} --build-dir=D:/Projects/3rd_Party/${LIB_TARGET_NAME}/build/${TOOLSETNAME} --with-chrono --with-system --with-filesystem --with-headers --with-date_time  --includedir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/include --libdir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/lib/${TOOLSETNAME} install")


set(INSTALL_DIR "${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}")

# if (ON)  # MSVC)  # in this moment not for MinGW enabled!!
if (MSVC)  # in this moment not for MinGW enabled!!
#===========================================
#-------------------
if(NOT EXISTS "${INSTALL_DIR}")

## b2 -j4 toolset=msvc link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=D:/link_libs/boost/boost-1.75.0 --build-dir=D:/Projects/3rd_Party/boost/build/msvc2019 --with-chrono --with-system --with-filesystem --with-headers --with-regex --with-date_time  --includedir=D:/link_libs/boost/boost-1.75.0/include --libdir=D:/link_libs/boost/boost-1.75.0/lib/msvc2019 install
message(STATUS "### ${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/src/${LIB_TARGET_NAME}")
message(STATUS "### ${${TARGET_CNAME}_BUILD_CMD}")


#-------------------
ExternalProject_Add(
   ${LIB_TARGET_NAME}
   GIT_REPOSITORY        "https://github.com/boostorg/boost.git"
   GIT_TAG               "${XCSOAR_${TARGET_CNAME}_VERSION}"

   PREFIX                "${${TARGET_CNAME}_PREFIX}"
#   BINARY_DIR            "${${TARGET_CNAME}_PREFIX}/build/${TOOLCHAIN}"
   INSTALL_DIR           "${INSTALL_DIR}"

   # PATCH_COMMAND         "bootstrap"
   CONFIGURE_COMMAND     bootstrap     ##  "${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/src/${LIB_TARGET_NAME}/TestOutput.cmd"

   # BUILD_COMMAND ${CMAKE_COMMAND} -E echo "Starting $<CONFIG> build"
   # COMMAND       "cd ../../src/${LIB_TARGET_NAME} & b2.exe -j4 toolset=${TOOLSET} variant=release link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION} --build-dir=D:/Projects/3rd_Party/${LIB_TARGET_NAME}/build/${TOOLSETNAME} --with-chrono --with-system --with-filesystem --with-headers --with-date_time  --includedir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/include --libdir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/lib/${TOOLSETNAME} install"
   # BUILD_COMMAND         
   # CONFIGURE_COMMAND     "../../src/${LIB_TARGET_NAME}/TestOutput.cmd ${LIB_TARGET_NAME} ${TOOLSET} ${TOOLSETNAME} ${XCSOAR_${TARGET_CNAME}_VERSION}"
#  BUILD_COMMAND "cd ../../src/${LIB_TARGET_NAME} & b2.exe -j4 toolset=${TOOLSET} variant=release link=static runtime-link=shared threading=multi address-model=64 --layout=versioned --prefix=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION} --build-dir=D:/Projects/3rd_Party/${LIB_TARGET_NAME}/build/${TOOLSETNAME} --with-chrono --with-system --with-filesystem --with-headers --with-date_time  --includedir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/include --libdir=${LINK_LIBS}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/lib/${TOOLSETNAME} install"
   BUILD_COMMAND "${${TARGET_CNAME}_BUILD_CMD}"

##     CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
##     "-DINSTALL_BIN_DIR:PATH=<INSTALL_DIR>/bin/${TOOLCHAIN}"
##     "-DINSTALL_LIB_DIR:PATH=<INSTALL_DIR>/lib/${TOOLCHAIN}"
    # BUILD_ALWAYS ${EP_BUILD_ALWAYS}
#    BUILD_ALWAYS ON
    BUILD_IN_SOURCE ON  ## ${EP_BUILD_IN_SOURCE}
    # WORKING_DIRECTORY ${EP_CMAKE}/${LIB_TARGET_NAME}/${XCSOAR_${TARGET_CNAME}_VERSION}/src/${LIB_TARGET_NAME}  # = src_dir!
)
#===========================================
set_target_properties(${LIB_TARGET_NAME} PROPERTIES FOLDER External)

endif()

endif(MSVC)
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

# TODO(aug): Move this to 3rd_party/boost.cmake!!!
### if(NOT Boost_DIR)
###   set(Boost_DIR "${BOOST_ROOT}/lib/${TOOLCHAIN}/cmake/Boost-1.77.0")  # /Boost_1_75_0")
###   message(STATUS "!!!!  Boost_DIR ${Boost_DIR}")
### endif()

set(Boost_USE_STATIC_LIBS         ON)
set(Boost_USE_MULTITHREADED       ON)
set(Boost_USE_STATIC_RUNTIME      OFF)
# set(Boost_DEBUG                   ON)
set(Boost_DEBUG                   OFF)
# set(BOOST_COMPONENTS system regex filesystem thread chrono date_time serialization)  #  network)
# set(BOOST_COMPONENTS date_time filesystem regex json)  #  network)
message(STATUS "!!! Boost_DIR = ${Boost_DIR} at $ENV{COMPUTERNAME}")
if (BOOST_COMPONENTS)
  # find_package(Boost 1.75 REQUIRED COMPONENTS ${BOOST_COMPONENTS})
  find_package(Boost 1.75 COMPONENTS ${BOOST_COMPONENTS})
else()
  find_package(Boost 1.75 REQUIRED)
endif()
      add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
      add_compile_definitions(BOOST_JSON_HEADER_ONLY)
      add_compile_definitions(BOOST_JSON_STANDALONE)
      add_compile_definitions(BOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON) 


