# message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
# get_filename_component(['TARGET_NAME', cmake_current_source_dir, 'NAME_WE'])
# organize the files in subdirectories
source_files = 
# foreach(['source_file', '${${TARGET_NAME}_SOURCES}'])
# string(['REPLACE', '${TARGET_NAME}/', '', 'source_file', source_file])
# list(['APPEND', 'SOURCE_FILES', source_file])
# get_filename_component(['src_path', source_file, 'DIRECTORY'])
if 'src_path'
  # string(['REPLACE', '/', '\\', 'src_path', src_path])
endif
# source_group(['Source\\${src_path}', 'FILES', source_file])
# message(STATUS "### ${src_path}  --- ${source_file}")
# endforeach()
# source_group(['Scripts', 'FILES', script_files])
# include_directories(cmake_current_source_dir)
# add_compile_definitions('CARES_STATICLIB')
TARGET_NAME_lib = library(target_name, xcsoar_lib_type, source_files, header_files, script_files)
# target_link_libraries([target_name, 'PUBLIC', cares_lib, 'system', 'time'])
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Libs'])
