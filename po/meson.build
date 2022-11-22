# message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
# get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
target_name = 'po-Files'
# include(CMakeSource.cmake)
# organize the files in subdirectories
# find_program(['PYTHON_APP', 'NAMES', 'python', 'REQUIRED'])
source_files = 
# set(PO_FILES        "${PROJECTGROUP_SOURCE_DIR}/po/de.po")
# file(['GLOB', 'PO_FILES', '*.po'])
script_files = 
mo_c_files = 
output_dir = [projectgroup_source_dir, '/output/data']
# get_filename_component(['_python_dir', python_app, 'DIRECTORY'])
msgfmt = '${_python_dir}/Tools/i18n/msgfmt.py'
if '1'
  # only if a po_file changed...
  # foreach(['po_file', po_files])
  # get_filename_component(['lang_name', po_file, 'NAME_WE'])
  # message(['STATUS', 'po_file  = ${po_file}  '])
  out_file = [output_dir, '/${lang_name}.mo.c']
  _mo_file = [output_dir, '/temp/${lang_name}.mo']
  # add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  # add_custom_command(['OUTPUT', out_file, 'COMMAND', python_app, msgfmt, '--output-file=${_mo_file}', po_file, 'COMMAND', python_app, 'tools/python/bin2c.py', _mo_file, output_dir, 'WORKING_DIRECTORY', projectgroup_source_dir, 'COMMENT', '${lang_name} ==> ${out_file}', 'DEPENDS', po_file])
  # list(['APPEND', 'MO_C_FILES', out_file])
  # endforeach()
endif
#! add_custom_target(${TARGET_NAME}
### add_custom_target(TARGET ${TARGET_NAME} PRE_BUILD
###     COMMAND ${CMAKE_COMMAND} -E make_directory  ${output_dir}/temp
###     WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
###     COMMENT Make dir '${output_dir}'
### )
if 'MSVC'
  # source_group(['po-Files', 'FILES', po_files])
endif
TARGET_NAME_lib = library(target_name, xcsoar_lib_type, mo_c_files, po_files)
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Data'])
