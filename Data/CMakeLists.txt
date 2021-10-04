cmake_minimum_required(VERSION 3.15)
message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")

  get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)


# include(CMakeSource.cmake)
# organize the files in subdirectories

#   set (SOURCE_FILES
#   #      test.cpp
#   )

#   set (BIN_FILES
#      Data/other/egm96s.dem
#      output/data/temp/AUTHORS.gz
#      output/data/temp/COPYING.gz
#      output/data/temp/NEWS.txt.gz
#   )

set (TXT_FILES
##   AUTHORS
##   COPYING
##   NEWS.txt
)

set (BIN_FILES
   AUTHORS.gz
   COPYING.gz
   NEWS.txt.gz
   Data/other/egm96s.dem
)

file(GLOB ICON_FILES        "icons/*.svg")
file(GLOB GRAPHIC_FILES     "graphic/*.svg")


set (SCRIPT_FILES )

# if (WINDOWS)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # list(APPEND CMAKE_PREFIX_PATH "D:/Programs/7-Zip")
  list(APPEND CMAKE_PROGRAM_PATH "D:/Programs/7-Zip")
#  list(APPEND CMAKE_PREFIX_PATH "C:/Program Files/7-Zip")
#  find_program(ZIP_APP 7z REQUIRED)
#  set(ZIP_APP D:/Programs/7-Zip/7z.exe)
endif()
find_program(ZIP_APP NAMES 7z gzip REQUIRED)
message(STATUS "Zip-App: '${ZIP_APP}'")
if (NOT ZIP_APP)
    set(ZIP_APP "D:/Programs/7-Zip/7z.exe")
    ## message(FATAL_ERROR "Missing Zip-App!")
    message(STATUS "Missing Zip-App!")
endif()

set(C_FILES)  # Reset to empty...
foreach(bin_file ${BIN_FILES})
   get_filename_component(c_file ${bin_file} NAME)
   set(c_file ${PROJECTGROUP_SOURCE_DIR}/output/data/${c_file}.c)
   # simular add_custom_command - but not the same ;-(
   if(${bin_file} MATCHES "\.gz$")
      string(REGEX REPLACE  "\.gz$"  "" txt_file ${bin_file})
      message(STATUS "MATCHES!!!! ${bin_file}  --- ${txt_file}")
      set(zip_file  "output/data/temp/${bin_file}")
      set (bin_file "${zip_file}")
      message(STATUS "OUTPUT: ${bin_file} --> ${c_file}")
      add_custom_command(OUTPUT ${c_file}
#       if (${bin_file} MATCHES "\.gz$")
            COMMAND echo "${ZIP_APP} a ${zip_file} ${txt_file}"
            COMMAND ${ZIP_APP} a ${zip_file}   ${txt_file}
#       endif()
       COMMAND echo "${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  output/data"
       COMMAND ${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  output/data
       WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
       COMMENT TextFile: ${bin_file}, 
    )
    else()
      message(STATUS "UNMATCHES!!!! ${bin_file}  --- ${txt_file}")
      message(STATUS "OUTPUT: ${bin_file} --> ${c_file}")
      add_custom_command(OUTPUT ${c_file}
         COMMAND echo "${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  output/data"
         COMMAND ${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  output/data
         WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
         COMMENT TextFile: ${bin_file}, 
      )
    endif()
    list(APPEND C_FILES ${c_file})
endforeach()

# message(FATAL_ERROR "STOP!!!!")

# Cmd-Line:
# perl -ne "print \"#define $1 $2\n\" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;" src/Resources.hpp >output/include/resource.h
set (PERL_CMD "-ne \"print \\\"\#define $1 $2\\n\\\" if /^MAKE_RESOURCE\\\(\(\\w+\), \(\\d+\)\\\)\;/\;\" ")
set(RESORCE_H ${PROJECTGROUP_SOURCE_DIR}/output/include/resource.h)
add_custom_command( # TARGET ${TARGET_NAME} PRE_BUILD
       OUTPUT ${RESORCE_H}
       COMMAND echo perl ${PERL_CMD} src/Resources.hpp
       COMMAND      perl ${PERL_CMD} src/Resources.hpp >${RESORCE_H}
       DEPENDS src/Resources.hpp output/include/dirstamp
       WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
       COMMENT ==== 
)

add_library(${TARGET_NAME}  ${XCSOAR_LIB_TYPE}
   ${C_FILES}
   ${HEADER_FILES}
   ${RESORCE_H}
)

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
# add_custom_command(OUTPUT output/data
               COMMAND ${CMAKE_COMMAND} -E make_directory  output/data
               WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
               COMMENT "Make dir 'output/data'"
)

if (0)
# Cmd-Line:
# perl -ne "print \"#define $1 $2\n\" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;" src/Resources.hpp >output/include/resource.h
set (PERL_CMD "-ne \"print \\\"\#define $1 $2\\n\\\" if /^MAKE_RESOURCE\\\(\(\\w+\), \(\\d+\)\\\)\;/\;\" ")
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
       COMMAND echo perl ${PERL_CMD} src/Resources.hpp
       COMMAND      perl ${PERL_CMD} src/Resources.hpp >output/include/resource.h
       DEPENDS src/Resources.hpp output/include/dirstamp
       WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
       COMMENT ==== 
)
endif()

# set_target_properties(${TARGET_NAME}X PROPERTIES FOLDER Data)

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Data
                                                #  ENABLE_EXPORTS TRUE
)
