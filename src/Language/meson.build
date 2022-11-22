# message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
# get_filename_component(['TARGET_NAME', cmake_current_source_dir, 'NAME_WE'])
# organize the files in subdirectories
source_files = 
# foreach(['source_file', '${${TARGET_NAME}_SOURCES}'])
# string(['REPLACE', '${TARGET_NAME}/', '', 'source_file', source_file])
# list(['APPEND', 'SOURCE_FILES', source_file])
# get_filename_component(['src_path', source_file, 'DIRECTORY'])
if 'MSVC'
  if 'src_path'
    # string(['REPLACE', '/', '\\', 'src_path', src_path])
  endif
endif
# source_group(['Source\\${src_path}', 'FILES', source_file])
# message(STATUS "### ${src_path}  --- ${source_file}")
# endforeach()
# include_directories(cmake_current_source_dir)
if 'MSVC'
  # source_group(['Script Files', 'FILES', script_files])
endif
### Translation Group
# collect the mo.c files from the po-files!
#### if(0)
####     set(MO_C_FILES)
####     get_filename_component(PO_DIR  ${PROJECTGROUP_SOURCE_DIR}/po     ABSOLUTE)
####     get_filename_component(output_dir  ${OUTPUT}/data ABSOLUTE)
####     get_filename_component(MSGFMT  ${PYTHON_APP} DIRECTORY ABSOLUTE )
####     
####     set(MSGFMT  ${MSGFMT}/Tools/i18n/msgfmt.py)
####     file(GLOB   LANGUAGE_PO_FILES   ${PO_DIR}/*.po) 
####     foreach(file_ ${LANGUAGE_PO_FILES})
####         get_filename_component(name_ ${file_} NAME_WE)  # de, bg, ...
####         set(_mo_file ${output_dir}/temp/${name_}.mo)
####         set(c_file  ${output_dir}/${name_}.mo.c)
####           list(APPEND MO_C_FILES ${c_file})
####         # get_filename_component(lang_name ${po_file}  NAME_WE)
####         message(STATUS "po_file  = ${file_}  ==> ${c_file}")
####         # set(out_file ${bin_file}  NAME_WE)
####           add_custom_command( # TARGET ${TARGET_NAME} POST_BUILD
####                 OUTPUT ${c_file}
####                 COMMAND echo ${PYTHON_APP} ${MSGFMT} --output-file=${_mo_file}  ${file_}
####                 COMMAND ${PYTHON_APP} ${MSGFMT} --output=${_mo_file}  ${file_}
####                 COMMAND ${PYTHON_APP} tools/python/bin2c.py ${_mo_file} ${output_dir}
####                 WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
####                 COMMENT ${name_}
####                 DEPENDS ${file_}
####         )
#### 
####     endforeach()
#### 
#### if(MSVC)
####     source_group("Translation\\po" FILES  ${LANGUAGE_PO_FILES})
#### endif()
#### endif()
TARGET_NAME_lib = library(target_name, xcsoar_lib_type, source_files, header_files, script_files)
# target_link_libraries([target_name, 'PUBLIC', 'Task', 'system', 'po-Files'])
# _Deprecated)
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Libs'])
# add_dependencies([target_name, 'po-Files'])
