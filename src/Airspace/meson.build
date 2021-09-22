# message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
# get_filename_component(['TARGET_NAME', cmake_current_source_dir, 'NAME_WE'])
#  project(${TARGET_NAME} CXX) # Your project name
# organize the files in subdirectories
source_files = 
# foreach(source_file ${${TARGET_NAME}_SOURCES})
# foreach(['source_file', _sources])
# string(['REPLACE', '${TARGET_NAME}/', '', 'source_file', source_file])
# list(['APPEND', 'SOURCE_FILES', source_file])
# get_filename_component(['src_path', source_file, 'DIRECTORY'])
if 'src_path'
  # string(['REPLACE', '/', '\\', 'src_path', src_path])
endif
# source_group(['Source\\${src_path}', 'FILES', source_file])
# message(STATUS "### ${src_path}  --- ${source_file}")
# endforeach()
if 'NOT', 'HEADER_FILES'
  #  STREQUAL ""
  # file(['GLOB_RECURSE', 'HEADER_FILES_TEMP', '${CMAKE_CURRENT_SOURCE_DIR}/*.h*'])
  # ;../*.hxx;../*.h
  header_files = 
  # foreach(['header_file', header_files_temp])
  # string(['REPLACE', '${CMAKE_CURRENT_SOURCE_DIR}/', '', 'header_file', header_file])
  # list(['APPEND', 'HEADER_FILES', header_file])
  # get_filename_component(['src_path', header_file, 'DIRECTORY'])
  if 'src_path'
    # string(['REPLACE', '/', '\\', 'src_path', src_path])
  endif
  # source_group(['Header\\${src_path}', 'FILES', header_file])
  ###    ## message(STATUS "### ### ${src_path}  --- ${header_file}" )
  # endforeach()
  ###     message(FATAL_ERROR "### ### Header-Liste  --- ${HEADER_FILES}" )
endif
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# include_directories(cmake_source_dir)
# include_directories([cmake_source_dir, '/Engine'])
TARGET_NAME_lib = library(target_name, xcsoar_lib_type, source_files, header_files, script_files)
# target_link_libraries([target_name, 'PUBLIC', 'io', 'Engine'])
# message(FATAL_ERROR "Stop!")
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Libs'])
