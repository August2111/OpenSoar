cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)
message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")

get_filename_component(PROJECTGROUP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
get_filename_component(PROJECTGROUP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} ABSOLUTE)
message(STATUS "*** PROJECTGROUP_SOURCE_DIR = '${PROJECTGROUP_SOURCE_DIR}'")
message(STATUS "*** PROJECTGROUP_BINARY_DIR = '${PROJECTGROUP_BINARY_DIR}'")

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_BINARY_DIR} NAME)
set (PROJECT_NAME "XCSoar-${PROJECT_NAME}")
set (PROJECT_NAME "XCSoar-August2111")
project(${PROJECT_NAME})
set(TARGET_NAME XCSoarAug)

include(build/cmake/xcsoar.cmake)
get_filename_component(OUTPUT output ABSOLUTE)

set(CMAKE_RC_COMPILER "D:/Programs/MinGW/mgw112/bin/windres.exe")
# set(BOOST_ROOT)
# noch falsch vom CMake-Create?
# set(LINK_LIBS "D:/Projects/link_libs")   # for c-ares???
# set(BOOST_ROOT ${LINK_LIBS}/boost/boost-1.77.0)

# 2021 clear the right place:
set(JASPER_OUTSIDE      OFF)
set(ZZIP_OUTSIDE        OFF)

find_program(PYTHON_APP NAMES  python REQUIRED)
find_program(PERL_APP   NAMES  perl   REQUIRED)
# message(FATAL_ERROR "Stop: ${PYTHON_APP}")

add_subdirectory(build/cmake)
add_subdirectory(Data)
add_subdirectory(po)

#---------------------------------------------------------
set(USE_MEMORY_CANVAS OFF)  # das ist hier auch falsch!!!!
if (USE_MEMORY_CANVAS)
       add_compile_definitions(USE_MEMORY_CANVAS)
       message(FATAL_ERROR "USE_MEMORY_CANVAS!!!!!")
else()
      ### message(FATAL_ERROR "GDI-Stop!  in ./CmakeLists")
      set(USE_GDI ON)  # das ist hier auch falsch!!!!
      add_compile_definitions(USE_GDI)
      ##  message(FATAL_ERROR USE_GDI!!!!!)
endif()
#---------------------------------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # important for folder structure f.e. in Visual Studio
# Setting Area ===================================================
include(ExternalProject)  # possible since 3.10

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # only by Makefile or Ninja generators: export compile commands
set(XCSOAR_LIB_TYPE STATIC)  # SHARED)

if (CMAKE_TOOLCHAIN_FILE)
  message(STATUS "CMAKE_TOOLCHAIN_FILE = '${CMAKE_TOOLCHAIN_FILE}'")
  # Die Variablen aus dem Toolchain-File Sind zwar hier nicht abrufbar, aber wirken auf Compiler uww.??? 
  # message(STATUS "CMAKE_SYSTEM_NAME = '${CMAKE_SYSTEM_NAME}'")
  # message(STATUS "CMAKE_CXX_COMPILER = '${CMAKE_CXX_COMPILER}'")
  # message(STATUS "CMAKE_C_COMPILER = '${CMAKE_C_COMPILER}'")
endif()

set(ENABLE_OPENGL ON)  # better outside????
set(ENABLE_OPENGL OFF)  # better outside????
# Target links to target GLUT::GLUT but the target was not

# cleanup 19.04.20: set(ASIO_GROUPING ON)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
  
message(STATUS "CMAKE_HOST_SYSTEM_NAME = ${CMAKE_HOST_SYSTEM_NAME} vs. CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME} ")

include(${PROJECTGROUP_SOURCE_DIR}/3rd_party/3rd_party.cmake)


set(SRC "${PROJECTGROUP_SOURCE_DIR}/src")
include(CMakeSource.cmake)

if (WIN32 AND MSVC)
  set(TARGET_NAME "XCSoarAug-MSVC")  # hardcoded yet
      message(STATUS "+++ System = WIN32 / MSVC!")

      add_definitions(-DNDEBUG)  # ansonsten erhalte ich im VS im Debug-Mode eine Menge Assertions bezüglich der 
      # fehlenden PNG-Bitmaps - und ich kann das Programm gar nicht debuggen ;-()
      add_compile_definitions(PROJECT_OUTPUT_FOLDER=${PROJECTGROUP_SOURCE_DIR}/output)

 #     if(NOT TOOLCHAIN)
        set(TOOLCHAIN msvc2019)
#      endif()
      add_compile_definitions(__MSVC__)
      add_compile_definitions(_UNICODE)
      add_compile_definitions(_AUG_MSC=1)
      add_compile_definitions(NO_ERROR_CHECK)  # EnumBitSet funktioniert m.E. noch nicht korrekt!!!!
      add_compile_definitions(WIN32_LEAN_AND_MEAN)
       # warning C4996: 'xxx': The POSIX name for this item is deprecated. Instead, use the ISO C and C++ conformant name: _wcsdup. See online help for details.
       # xxx: wcscpy, wcsdup, strtok, strcpy, strdup, ....
      add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
      # add_compile_definitions(/std:c++17 /Zc:__cplusplus)
      add_definitions(/std:c++latest /Zc:__cplusplus)
      add_definitions(/wd5030)

      # add_definitions(/Zc:wchar_t)
      set(LIB_EXTENSION "lib")
      set(PRE_LIB "")

      include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporary data!
      if (ON OR WIN64)  # momentan kein Flag für 64bit verfügbar!
          add_compile_definitions(_AMD64_)
      else()
          message(FATAL_ERROR "Error: WIN32 not implemented?")
      endif()
# boost.cmake      set(BOOST_ROOT ${LINK_LIBS}/boost/boost-1.77.0)
      set(FREEGLUT_LIB_DIR "${LINK_LIBS}/freeglut-MSVC-3.0.0-2/freeglut")
      set(SODIUM_LIB "${LINK_LIBS}/libsodium/x64/Release/v142/static/libsodium.lib")
      add_compile_definitions(SODIUM_STATIC=1)  # MSCV only...

      add_compile_definitions(CURL_STATICLIB)
      add_compile_definitions(LDAP_STATICLIB)


      set(CRYPTO_LIB Crypt32.lib)
      set (CARES_LIB ${LINK_LIBS}/c-ares/${XCSOAR_CARES_VERSION}/lib/${TOOLCHAIN}/cares.lib)
      # add_compile_definitions(_WINSOCKAPI_)
    set(USE_MEMORY_CANVAS OFF)
elseif(WIN32 AND MINGW)
  set(TARGET_NAME "XCSoarAug-MinGW")  # hardcoded
      # Only MinGW / GCC:
      message(STATUS "+++ System = WIN32 / MinGW (${TOOLCHAIN})!")
      if(NOT TOOLCHAIN)
        set(TOOLCHAIN mgw112)
        # set(TOOLCHAIN mgw82)
        # message(FATAL_ERROR "No Toolchain!")
      endif()
      set(LIB_EXTENSION "a")
      set(PRE_LIB "lib")

      # no lib structure, this is failing on GCC  (serial order)???
# ??? #      set(XCSOAR_LIB_TYPE WITHOUT)

#      set(Boost_COMPILER "${TOOLCHAIN}")  # necessary: changed mgw as boost compiler to mgw73!
#      set(Boost_COMPILER "mgw7")  # necessary: changed mgw as boost compiler to mgw73!
    
      # if(${TOOLCHAIN}  STREQUAL mgw73)
      #   # include_directories("${THIRD_PARTY}/qt/Qt5.14.0/Tools/mingw730_64/x86_64-w64-mingw32/include")
      #   add_definitions(-DBOOST_AUTO_LINK_SYSTEM)         # -DBOOST_AUTO_LINK_VERSIONED)
      # endif()
      if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
         include_directories("D:/Programs/MinGW/${TOOLCHAIN}/include")
         #  later: include_directories("${PROJECTGROUP_SOURCE_DIR}/output/include")
      endif()

      include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporory data!

      add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
      # disable WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      add_compile_definitions(BOOST_MATH_DISABLE_DEPRECATED_03_WARNING=ON)

      # add_compile_definitions(std=c++17) ### ??? Only a test on 11.05.2020
# noch einmal ausblenden?
      #   add_definitions(-std=c++17) ### Das setzt für c- und rc-Compiler ein falsches Argument!
###      set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17")
      set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20")
      set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines")
      set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}")

      # sequence from 'targets.mk' 
        add_compile_definitions(__MINGW__)
        add_compile_definitions(_AUG_MGW=1)
        add_compile_definitions(HAVE_MSVCRT)
        add_compile_definitions(_UNICODE)
        add_compile_definitions(UNICODE)
        add_compile_definitions(STRICT)
        add_compile_definitions(WIN32)  # XCSoar used this instead of _WIN32
        add_compile_definitions(_USE_MATH_DEFINES)   # necessary under C++17!
        add_compile_definitions(ZZIP_1_H)   # definition of uint32_t and Co.!

#      set(CMAKE_C_COMPILE_OBJECT "${CMAKE_C_COMPILER} -c <FLAGS> -o <OBJECT>")
#      set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

#    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 -lgdi32 -lgdiplus -lcrypt32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -v")
    set(FREEGLUT_LIB_DIR "${LINK_LIBS}/freeglut-MinGW-3.0.0-1/freeglut")
    set(SODIUM_LIB  ${LINK_LIBS}/libsodium/libsodium-win64/lib/libsodium.a)

    set(SSL_LIB ${LINK_LIBS}/openssl/openssl_1.1.1i/lib/mgw73/libssl.a)
    set(CRYPTO_LIB ${LINK_LIBS}/openssl/openssl_1.1.1i/lib/mgw73/libcrypto.a)
     set(CARES_LIB ${LINK_LIBS}/c-ares/${XCSOAR_CARES_VERSION}/lib/${TOOLCHAIN}/libcares.a)
elseif(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  set(TARGET_NAME "XCSoarAug-Clang")  # hardcoded
      message(STATUS "+++ System = WIN32 / Clang!")
      if(NOT TOOLCHAIN)
        set(TOOLCHAIN clang)
      endif()
      # add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)
      # add_compile_definitions(_AUG_MSC=1)  ### really???  ###
      # add_compile_definitions(_UNICODE)

        add_compile_definitions(__CLANG__)
        add_compile_definitions(_AUG_CLANG=1)
        add_compile_definitions(HAVE_MSVCRT)
        add_compile_definitions(_UNICODE)
        add_compile_definitions(UNICODE)
        add_compile_definitions(STRICT)
        add_compile_definitions(_USE_MATH_DEFINES)   # necessary under C++17!

      # add_compile_definitions(std:c++11) ### Only a test on 11.05.2020
# ???      add_definitions(-std=c++17) ### ??? Only a test on 11.05.2020
      # add_compile_definitions(-std=c++17) ### ??? Only a test on 11.05.2020
      set(LIB_EXTENSION "lib")
      set(PRE_LIB "")
      include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporary data!
      if (ON OR WIN64)  # momentan kein Flag verfügbar!
          add_compile_definitions(_AMD64_)
      else()
          message(FATAL_ERROR "Error: WIN32 not implemented?")
      endif()
    set(Boost_COMPILER "clang10")  # necessary: changed clang8 as boost compiler to clang!
#    set(Boost_DIR "${BOOST_ROOT}/lib/${Boost_COMPILER}/cmake/Boost-1.77.0")
     set(SSL_LIB )
     set(CRYPTO_LIB )
     set(CARES_LIB ${LINK_LIBS}/c-ares/${XCSOAR_CARES_VERSION}/lib/${TOOLCHAIN}/cares.lib)


elseif(WIN32 AND CLANG)
   message(FATAL_ERROR "+++ Unbekanntes System: CLANG!")
elseif(WIN32 AND NINJA)
   message(FATAL_ERROR "+++ Unbekanntes System: NINJA!")
elseif(UNIX)
      message(STATUS "+++ System = LINUX / GCC(?) (${TOOLCHAIN})!")
      
      set(LIB_EXTENSION "a")
      set(PRE_LIB "lib")

      # no lib structure, this is failing on GCC  (serial order)???
      # set(XCSOAR_LIB_TYPE WITHOUT)
      if(MINGW)
         set(ENABLE_OPENGL OFF)  # das ist hier MinGWCrossCompiling für Windows! 
         message(FATAL_ERROR "Stop MinGW")
      else()
         set(ENABLE_OPENGL OFF)
      endif()   

      if (ON) # 2020-05-20: inserted on pcderad0633 regarding missing 'tchar.h'
        add_compile_definitions(HAVE_MSVCRT)
        add_compile_definitions(_UNICODE)
        add_compile_definitions(UNICODE)
        add_compile_definitions(STRICT)
      endif()
      if($ENV{USER} STREQUAL "pcderad0633") 
         # why?
         set(Boost_COMPILER "gcc9") 
      	 add_compile_definitions(__LITTLE_ENDIAN=1)  # have to be defined!
      	 add_compile_definitions(__BIG_ENDIAN=2)
      else()
         set(Boost_COMPILER "gcc7")  # necessary: ??
      endif()

      # set(Boost_COMPILER "${TOOLCHAIN}")  # necessary: changed mgw as boost compiler to mgw73!
      
     
      # include_directories("D:/Programs/MinGW/${TOOLCHAIN}/include")
      include_directories("${PROJECTGROUP_SOURCE_DIR}/temp/data")  # temporary data!
      include_directories("${PROJECTGROUP_SOURCE_DIR}/src/unix")   # temporary data!

      set(Boost_DIR "$ENV{HOME}/Projects/link_libs/boost/boost-1.77.0/lib/unix/cmake/Boost-1.77.0") 
  
      set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -v")
      # message(FATAL_ERROR "+++ Jetzt bin ich im Linux: !")
     set(SSL_LIB )  # empty ?
     set(CRYPTO_LIB )  #empty ?
     set (CARES_LIB ${LINK_LIBS}/c-ares/${XCSOAR_CARES_VERSION}/lib/${TOOLCHAIN}/cares.lib)

elseif(ANDROID)
    message(STATUS "+++ System = ANDROID / ${ANDROID_ABI}!")
    set(Boost_COMPILER "clang")  # necessary: changed clang8 as boost compiler to clang!
else()
   message(FATAL_ERROR "+++ Unbekanntes System: ${CMAKE_SYSTEM}!")
endif()

if (MSVC)  # Themes for MSVC only!
    add_compile_definitions(GNU_CONST=)
    add_compile_definitions(GNU_PURE=)
else()   ### MSVC
    add_compile_definitions("GNU_CONST=[[gnu::const]]")
    add_compile_definitions("GNU_PURE=[[gnu::pure]]")
endif()  ### MSVC


include_directories("${PROJECTGROUP_SOURCE_DIR}/src/io") # only regarding Logfile!
include_directories("${PROJECTGROUP_SOURCE_DIR}/output/include")

# git commit hash macro
execute_process(
  COMMAND  git rev-parse --short HEAD
  WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_ID
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
# string(SUBSTRING ${GIT_COMMIT_HASH} 0 8  GIT_COMMIT_ID)
add_compile_definitions("GIT_COMMIT_ID=\"${GIT_COMMIT_ID}\"")

# 3rd Party !!!!
#=============================
#    set(THIRDPARTY_INCLUDES)
    add_subdirectory(3rd_party)
    message(STATUS "### THIRDPARTY_INCLUDES = '${THIRDPARTY_INCLUDES}'")

    include_directories(
         ${LUA_INCLUDE_DIR}
         ${CURL_INCLUDE_DIR}
#  Excluded 2021    ${JASPER_INCLUDE_DIR}
         ${MAPSERVER_INCLUDE_DIR}
         ${ZLIB_INCLUDE_DIR}
         ${LIBPNG_INCLUDE_DIR}
         ${C-ARES_INCLUDE_DIR}
      )
#      include_directories("${THIRD_PARTY}/glut/glut-3.7/include")  # neu am 27.03
if (JASPER_OUTSIDE)  # 2021
    # external source in 3rd party lib
    include_directories(${JASPER_INCLUDE_DIR})
endif (JASPER_OUTSIDE)
if (ZZIP_OUTSIDE)  # 2021
    # external source in 3rd party lib
    include_directories(${ZZIP_INCLUDE_DIR})
endif (ZZIP_OUTSIDE)

#####################################################################################
#####################################################################################
# add_compile_definitions(USE_MEMORY_CANVAS)
add_compile_definitions(NOMINMAX)
# sadd_compile_definitions(_AUG=1)
# nur einmal definieren!!! add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)

# add version string to xcsoar:
file(READ "VERSION.txt" XCSOAR_VERSION)
string(STRIP ${XCSOAR_VERSION} XCSOAR_VERSION)
add_compile_definitions("XCSOAR_VERSION=\"${XCSOAR_VERSION}\"")

# add_definitions(/std:c++11 /Zc:__cplusplus)
if(MSVC)
# add_compile_definitions(_X86_) #  _IA64_)
##  add_definitions(/std:c++14 /Zc:__cplusplus)
# TARGET_ARCHITECTURE: _AMD64_ _X86_ _IA64_ _ARM_ _ARM64_
endif()
# USE_MEMORY_CANVAS
# USE_GDI

if(ENABLE_OPENGL)
    # set(FreeGLUT_DIR ${LINK_LIBS}/glut/freeglut-3.2.1/lib/msvc2019/cmake/FreeGLUT)
    # find_package(FreeGLUT REQUIRED)
    find_package(OpenGL REQUIRED)
    # find_package(OpenCL REQUIRED)
    # set(GLEW_ROOT D:/link_libs/glew/glew-2.1.0/lib/Release/x64)
    # find_package(GLEW REQUIRED)
    
    if(NOT TARGET OpenGL::GLU)
        message(FATAL_ERROR "GLU could not be found")
    endif(NOT TARGET OpenGL::GLU)
    
    message(STATUS "glu: ${OPENGL_glu_LIBRARY}")
    
    get_target_property(GLU_IMPORTED_LIBNAME OpenGL::GLU IMPORTED_LIBNAME)
    message(STATUS "glu imported libname: ${GLU_IMPORTED_LIBNAME}")
    add_compile_definitions(ENABLE_OPENGL)
    add_compile_definitions(USE_WIN32_RESOURCES USE_WINUSER)

    if (ANDROID)
       include_directories(D:/Programs/Android/android-ndk-r21/sysroot/usr/include/GLES2)
    endif()
    include_directories(${Boost_INCLUDE_DIR}/boost/compute/interop)

    include_directories(${FREEGLUT_LIB_DIR}/include)
    include_directories($ENV{OCL_ROOT}/include)
    include_directories(D:/Programs/OCL_SDK_Light/include) # alternative???
###    ${OCL_ROOT}\include
###    ${OCL_ROOT}\lib\x86
###    ${OCL_ROOT}\lib\x86_64


    message(FATAL_ERROR "OpenGL-Stop!  OpenGL_INCLUDE_DIR = ${OpenGL_INCLUDE_DIR}")
else()
# Kein OpenGL!!!!!!!!!!!!!!
##    if (USE_MEMORY_CANVAS)
##       add_compile_definitions(USE_MEMORY_CANVAS)
##    else()
####      message(FATAL_ERROR "GDI-Stop!  in ./CmakeLists")
####      message(FATAL_ERROR "OpenGL-Stop!  OpenGL_INCLUDE_DIR = ${OpenGL_INCLUDE_DIR}")
##      add_compile_definitions(USE_GDI)
      add_compile_definitions(USE_WIN32_RESOURCES USE_WINUSER)
##    endif()
endif()
#####################################################################################
#####################################################################################


include_directories( ${LINK_LIBS}/libsodium/include)
include_directories( ${Boost_INCLUDE_DIR})
link_directories(${BOOST_ROOT}/lib)

get_filename_component(TEMP_INCLUDES ${PROJECTGROUP_BINARY_DIR}/../include ABSOLUTE)
include_directories(
    ${PROJECTGROUP_SOURCE_DIR}/src
    ${PROJECTGROUP_SOURCE_DIR}/src/Engine
    ${TEMP_INCLUDES}
    # better with find_package
#    ${PROJECTGROUP_BINARY_DIR}/output/src/${XCSOAR_CURL_VERSION}/include

    ${Boost_INCLUDE_DIR}/boost/predef/other

    ${THIRD_PARTY}/glfw/glfw-3.3.2.bin.WIN64/include
    ${LINK_LIBS}/glm/glm-0.9.9/include
)

if (JASPER_OUTSIDE)  # 2021
    # before add_subdirectories!!!
    add_compile_definitions(JASPER_OUTSIDE)
endif (JASPER_OUTSIDE)


set(SOURCE_FILES )

# message(FATAL_ERROR "Test-Stop!")


# branch cmake    add_subdirectory(src/_Deprecated)
    add_subdirectory(src/Airspace)
    add_subdirectory(src/Atmosphere)
    add_subdirectory(src/Audio)
    add_subdirectory(src/Blackboard)
    add_subdirectory(src/co)
    add_subdirectory(src/Computer)
    add_subdirectory(src/Cloud)
    add_subdirectory(src/CrossSection)
    add_subdirectory(src/Engine)
    add_subdirectory(src/event)
    add_subdirectory(src/FLARM)
    add_subdirectory(src/Form)
    add_subdirectory(src/Formatter)
    add_subdirectory(src/Gauge)
    add_subdirectory(src/Geo)
    add_subdirectory(src/Hardware)
    add_subdirectory(src/IGC)
    add_subdirectory(src/InfoBoxes)
    add_subdirectory(src/Input)
    add_subdirectory(src/Job)
    add_subdirectory(src/json)
    add_subdirectory(src/Language)
    add_subdirectory(src/Logger)
    add_subdirectory(src/Look)
    add_subdirectory(src/lua)
    add_subdirectory(src/Markers)
    add_subdirectory(src/Math)
    add_subdirectory(src/Menu)
    add_subdirectory(src/Monitor)
    add_subdirectory(src/net)
    add_subdirectory(src/NMEA)
    add_subdirectory(src/Operation)
    add_subdirectory(src/system)
    add_subdirectory(src/Plane)
    add_subdirectory(src/Polar)
    add_subdirectory(src/Profile)
    add_subdirectory(src/Projection)
    add_subdirectory(src/Renderer)
    add_subdirectory(src/Replay)
    add_subdirectory(src/Repository)
    add_subdirectory(src/Screen)
    add_subdirectory(src/Task)
    add_subdirectory(src/TeamCode)
    add_subdirectory(src/Terrain)
if (NOT JASPER_OUTSIDE)  # 2021
    add_subdirectory(src/Terrain/jasper)  # 2021
endif (JASPER_OUTSIDE)
    add_subdirectory(src/thread)
    add_subdirectory(src/time)
    add_subdirectory(src/Topography)
    add_subdirectory(src/ui    )
    add_subdirectory(src/UIUtil)
    add_subdirectory(src/Units)
    add_subdirectory(src/util)
    add_subdirectory(src/Waypoint)
    add_subdirectory(src/Weather)
    add_subdirectory(src/Widget)
    add_subdirectory(src/XML)

    add_subdirectory(src/Device)
    add_subdirectory(src/Dialogs)
    add_subdirectory(src/io)
    add_subdirectory(src/MapWindow)
    add_subdirectory(src/Tracking)
    add_subdirectory(src/contest)
if(NOT ZZIP_OUTSIDE)
    add_subdirectory(src/zzip)   # aug: new!!
endif()
  add_subdirectory(src)   # libXCSoar!

# # #   ### Translation Group
# # #    # if (NOT UNIX)
#xxx        set_source_files_properties(${PROJECTGROUP_SOURCE_DIR}/temp/data/memory_fields.c PROPERTIES LANGUAGE C)
#xxx        list(APPEND SOURCE_FILES "${PROJECTGROUP_SOURCE_DIR}/temp/data/memory_fields.c")


if (MINGW)
else()  # TODO(aug) fuer MinGW erst einmal weglassen!
endif()

list(APPEND SOURCE_FILES "src/XCSoar.cpp")
list(APPEND SOURCE_FILES "src/Version.cpp")
if (MSVC)
  list(APPEND SOURCE_FILES "Data/XCSoar.def")
  list(APPEND SOURCE_FILES "Data/XCSoar2.rc")
else()
  list(APPEND SOURCE_FILES "Data/XCSoar.rc")
endif()

list(APPEND SOURCE_FILES "CMakeSource.cmake")

#==========================================================
#==========================================================

# preparing target_link_libraries!
list(APPEND XCSOAR_LINK_LIBRARIES  libXCSoar     ${XCSOAR_SOURCE_LISTS})

# TODO(aug): Cleaning up this code!!!

# External Libs
# ATTETION: Only WorkAround: Better definition inside (27.12.2021)
set(CURL_LIB "D:/Projects/link_libs/curl/curl-7.75.0/lib/msvc2022/libcurl.lib")
set(MAPSERVER_LIB "D:/Projects/link_libs/mapserver/mapserver-xcsoar/lib/msvc2019/mapserver.lib")
set(LIBPNG_LIB "D:/Projects/link_libs/libpng/libpng-1.6.37/lib/msvc2019/libpng16.lib")
set(CARES_LIB "D:/Projects/link_libs/c-ares/c-ares-1.17.1/lib/msvc2019/cares.lib")

list(APPEND XCSOAR_LINK_LIBRARIES
   ${LUA_LIB}
   ${JASPER_LIB}  # internal or external...
# ExternalProjectTarget
   ${CURL_LIB}
   ${MAPSERVER_LIB}
   ${LIBPNG_LIB}
# ExternalProjectTarget
   ${ZLIB_LIB}
   ${ZZIP_LIB}    # internal or external...
   ${SODIUM_LIB}  # new at 06/2020
   ${SSL_LIB}  # new at 03/2021
   ${CRYPTO_LIB}  # new at 03/2021
   ${CARES_LIB}  # new at 03/2021
   
   ${Boost_LIBRARIES}
)
### list(APPEND XCSOAR_LINK_LIBRARIES
### #   ${ZZIP_LIB}
###    zzip
### #    libzzip
### )
if (WIN32)  # Windows only:
    if (MSVC)
      list(APPEND XCSOAR_LINK_LIBRARIES
        msimg32.lib
        winmm.lib

        ws2_32.lib   # das ist nur für MinGW?
        gdiplus
    )
    elseif (MINGW)
      list(APPEND XCSOAR_LINK_LIBRARIES
        msimg32
        winmm
        # dl
        pthread
        stdc++
        user32
        gdi32
        gdiplus
        ws2_32  # MinGW!
        mswsock
        # android: intl
        m
        # nicht in Linux: log
        # not android , but linux! rt
        kernel32
        # ?? msvcrt32
        shell32
        gcc_s
        # ldap32
      )
    endif()
else()
###  ..  other system!
endif()
  #==========================================================
  #==========================================================

set(SCRIPT_FILES
    CMakeSource.cmake
    VERSION.txt
    NEWS.txt
    meson.build
    "TaskList MSVC.md"
)

if (ANDROID)  # library!
   add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
elseif(MINGW)
   add_executable(${TARGET_NAME}     ${SOURCE_FILES} ${SCRIPT_FILES} ${EXT_PROJ}) #  ${XCSOAR_LINK_LIBRARIES})
else()        # executable!
   message(STATUS  "### add_executable(${TARGET_NAME}     ${SOURCE_FILES} ${SCRIPT_FILES} ${EXT_PROJ}")
   add_executable(${TARGET_NAME}     ${SOURCE_FILES} ${SCRIPT_FILES} ${EXT_PROJ})
endif()


# target_link_libraries(${TARGET_NAME}
list(APPEND XCSOAR_LINK_LIBRARIES
#    ${Boost_LIBRARIES}
#    libXCSoar
### eay
### ssl
### z
) # Reihenfolge is very important!!!!

if (MSVC)
  list(APPEND XCSOAR_LINK_LIBRARIES
    ${LINK_LIBS}/openssl/OpenSSL-Win64/lib/VC/static/libssl64MDd.lib
    ${LINK_LIBS}/openssl/OpenSSL-Win64/lib/VC/static/libcrypto64MDd.lib
    Crypt32.lib
    # CryptNet.lib
    # cryptdll.lib
  )
endif()

# foreach(lib ${XCSOAR_LINK_LIBRARIES})
#    message(STATUS "--- XCSOAR_LINK_LIB: ${lib}")
# endforeach()

target_link_libraries(${TARGET_NAME} PUBLIC ${XCSOAR_LINK_LIBRARIES})
if(MSVC)
  target_link_options(${TARGET_NAME} PUBLIC "/SUBSYSTEM:WINDOWS")
endif()

if(ENABLE_OPENGL)
    target_link_libraries(${TARGET_NAME}
        PRIVATE   # PUBLIC
        OpenGL::GL
        OpenGL:GLU # single colon!
        # GLUT::GLUT
        # ${FREEGLUT_LIB_DIR}/lib/x64/${PRE_LIB}freeglut_static.${LIB_EXTENSION} # MinGW
        ${FREEGLUT_LIB_DIR}/lib/x64/${PRE_LIB}freeglut.${LIB_EXTENSION}  # MSVC, but with DLL!
        # ${OCL_ROOT}\lib\x86_64
        # FreeGLUT::FreeGLUT
        ###${THIRD_PARTY}/glfw/glfw-3.3.2.bin.WIN64/lib-vc2019/glfw3.${LIB_EXTENSION}
        ### ${LINK_LIBS}/glm/glm-0.9.9/lib/msvc2019/glm_static.
        ###${LINK_LIBS}/glut/freeglut-3.2.1/lib/msvc2019/freeglut.${LIB_EXTENSION}   # libpng.${LIB_EXTENSION}"
    )
endif()

        # add_custom_target(translation_copy    COMMAND  "${CMAKE_COMMAND}" -E copy_if_different
        add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${directory} ${CMAKE_CURRENT_BINARY_DIR}/po
        )

foreach(translation ${TRANSLATIONS})
        add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECTGROUP_SOURCE_DIR}/src/_Deprecated/${translation}.mo ${CMAKE_CURRENT_BINARY_DIR}/po/${translation}.mo
            # TODO(aug): later do the creation here...
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECTGROUP_SOURCE_DIR}/src/_Deprecated/${translation}.mo.c ${CMAKE_CURRENT_BINARY_DIR}/po/${translation}.mo.c
        )
#    if(NOT MSVC)
#            list(APPEND SOURCE_FILES "${PROJECTGROUP_SOURCE_DIR}/output/data/${translation}.mo.c")
#    endif()
endforeach()

if (OFF)  # only if to build
    if (JASPER_OUTSIDE)  # 2021
        add_dependencies(${TARGET_NAME} curl lua libpng) # 2021 excluded (!): jasper 
    else(JASPER_OUTSIDE)
        add_dependencies(${TARGET_NAME} curl lua libpng jasper)
    endif (JASPER_OUTSIDE)
        add_dependencies(${TARGET_NAME} zlib)
        add_dependencies(${TARGET_NAME} mapserver zzip)   # missing: boost xmlparser
#        add_dependencies(${TARGET_NAME} mapserver libzzip)   # missing: boost xmlparser
        add_dependencies(${TARGET_NAME} googletest )   # missing: boost xmlparser
    # missing: add_dependencies(${TARGET_NAME} boost)
    # missing: add_dependencies(${TARGET_NAME} xmlparser)
endif()

# add_dependencies(${TARGET_NAME} libXCSoar)   # missing: boost xmlparser
# Test-Folder !!!!
#=============================
    add_subdirectory(test/src)
    add_subdirectory(tools)
    message(STATUS "### Test  =====================")

