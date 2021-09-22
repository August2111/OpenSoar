  # message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
  # get_filename_component(['TARGET_NAME', cmake_current_source_dir, 'NAME_WE'])
  project('TARGET_NAME', default_options : ['default_library=static'])
  ###  CXX) # Your project name
  # include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeSource.cmake)
  if 'EXISTS', 'CMakeSource.cmake'
    # message(FATAL_ERROR "Stop: ${CMAKE_CURRENT_SOURCE_DIR}/CMakeSource.cmake found")
  elif 'EXISTS', cmake_current_source_dir, '/CMakeSource.cmake'
    # message(['STATUS', '!!! ${CMAKE_CURRENT_SOURCE_DIR}/CMakeSource.cmake found'])
  endif
  # organize the files in subdirectories
  # list(APPEND ${TARGET_NAME}_SOURCES  ${TARGET_NAME}/jpc_rtc.cpp)
  source_files = 
  # foreach(['source_file', '${${TARGET_NAME}_SOURCES}'])
  # list(['APPEND', 'SOURCE_FILES', source_file])
  # set_source_files_properties(${source_file} PROPERTIES LANGUAGE CXX)
  # string(REPLACE "${TARGET_NAME}/" "" source_file ${source_file})
  # string(['REPLACE', '${CMAKE_CURRENT_SOURCE_DIR}/', '', 'source_file', source_file])
  # get_filename_component(['src_path', source_file, 'DIRECTORY'])
  if 'src_path'
    # string(['REPLACE', '/', '\\', 'src_path', src_path])
  endif
  # source_group(['Source\\${src_path}', 'FILES', '${source_file}'])
  ### hide: message(STATUS "### ${src_path}  --- ${source_file}")
  # endforeach()
  if 'NOT', 'HEADER_FILES'
    #  STREQUAL ""
    # with jasper...:  
    # file(['GLOB_RECURSE', 'HEADER_FILES_TEMP', '${CMAKE_CURRENT_SOURCE_DIR}/*.h*'])
    # ;../*.hxx;../*.h
    # without jasper...:  file(GLOB   HEADER_FILES_TEMP   "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")  # ;../*.hxx;../*.h
    #    message(FATAL_ERROR "### ### Header-Liste  --- ${HEADER_FILES_TEMP}" )
    header_files = 
    # foreach(['header_file', header_files_temp])
    # string(['REPLACE', '${CMAKE_CURRENT_SOURCE_DIR}/', '', 'header_file', header_file])
    # list(['APPEND', 'HEADER_FILES', header_file])
    # get_filename_component(['src_path', header_file, 'DIRECTORY'])
    if 'src_path'
      # string(['REPLACE', '/', '\\', 'src_path', src_path])
    endif
    # source_group(['Header\\${src_path}', 'FILES', header_file])
    ## message(STATUS "### ### ${src_path}  --- ${header_file}" )
    # endforeach()
    ##  message(FATAL_ERROR "### ### Header-Liste  --- ${HEADER_FILES}" )
  endif
  # get_filename_component(['INCLUDE_DIR', cmake_current_source_dir, '/..', 'ABSOLUTE'])
  # include_directories(include_dir)
  # include_directories(${3rd_Party}/jasper/Jasper-2.0.0/src/jasper)
  # include_directories(${3rd_Party}/_install/jasper/JasPer_2.0.0/include)
  TARGET_NAME_lib = library(target_name, xcsoar_lib_type, source_files, header_files, script_files)
  # target_link_libraries([target_name, 'PUBLIC', zzip_lib])
  # message(FATAL_ERROR "Stop!")
  # set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Libs'])
  if 'ON'
    # add_dependencies(${TARGET_NAME} jasper zzip)   #XCSoar Dialogs)
  endif
