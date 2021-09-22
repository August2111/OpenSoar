# message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
# get_filename_component(['TARGET_NAME', cmake_current_source_dir, 'NAME_WE'])
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
txt_files = ''
bin_files = ['AUTHORS.gz', 'COPYING.gz', 'NEWS.txt.gz', 'Data/other/egm96s.dem']
# file(['GLOB', 'ICON_FILES', 'icons/*.svg'])
# file(['GLOB', 'GRAPHIC_FILES', 'graphic/*.svg'])
script_files = '' 
# if (WINDOWS)
### if cmake_system_name.contain('Windows')
if host_machine.system().contains('Windows')
  # list(APPEND CMAKE_PREFIX_PATH "D:/Programs/7-Zip")
  # list(['APPEND', 'CMAKE_PROGRAM_PATH', 'D:/Programs/7-Zip'])
  #  list(APPEND CMAKE_PREFIX_PATH "C:/Program Files/7-Zip")
  #  find_program(ZIP_APP 7z REQUIRED)
  #  set(ZIP_APP D:/Programs/7-Zip/7z.exe)
endif
# find_program(['ZIP_APP', 'NAMES', '7z', 'gzip', 'REQUIRED'])
# message(['STATUS', 'Zip-App: '${ZIP_APP}''])
if not true  # 'ZIP_APP'
  zip_app = 'D:/Programs/7-Zip/7z.exe'
  ## message(FATAL_ERROR "Missing Zip-App!")
  # message(['STATUS', 'Missing Zip-App!'])
endif
c_files = ''
# Reset to empty...
# foreach(['bin_file', bin_files])
bin_file = 'irgendwas'
# get_filename_component(['c_file', bin_file, 'NAME'])
c_file = [projectgroup_source_dir, '/output/data/${c_file}.c']
# simular add_custom_command - but not the same ;-(
if bin_file.contains('\.gz$')
  # string(['REGEX', 'REPLACE', '\.gz$', '', 'txt_file', bin_file])
  # message(['STATUS', 'MATCHES!!!! ${bin_file}  --- ${txt_file}'])
  zip_file = 'output/data/temp/${bin_file}'
  bin_file = '${zip_file}'
  # message(['STATUS', 'OUTPUT: ${bin_file} --> ${c_file}'])
  # add_custom_command(['OUTPUT', c_file, 'COMMAND', 'echo', '${ZIP_APP} a ${zip_file} ${txt_file}', 'COMMAND', zip_app, 'a', zip_file, txt_file, 'COMMAND', 'echo', '${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  output/data', 'COMMAND', python_app, 'tools/python/bin2c.py', bin_file, 'output/data', 'WORKING_DIRECTORY', projectgroup_source_dir, 'COMMENT', 'TextFile:', bin_file, ','])
else
  # message(['STATUS', 'UNMATCHES!!!! ${bin_file}  --- ${txt_file}'])
  # message(['STATUS', 'OUTPUT: ${bin_file} --> ${c_file}'])
  # add_custom_command(['OUTPUT', c_file, 'COMMAND', 'echo', '${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  output/data', 'COMMAND', python_app, 'tools/python/bin2c.py', bin_file, 'output/data', 'WORKING_DIRECTORY', projectgroup_source_dir, 'COMMENT', 'TextFile:', bin_file, ','])
endif
# list(['APPEND', 'C_FILES', c_file])
# endforeach()
# message(FATAL_ERROR "STOP!!!!")
# Cmd-Line:
# perl -ne "print \"#define $1 $2\n\" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;" src/Resources.hpp >output/include/resource.h
perl_cmd = '-ne \"print \\\"\#define $1 $2\\n\\\" if /^MAKE_RESOURCE\\\(\(\\w+\), \(\\d+\)\\\)\;/\;\" '
resorce_h = [projectgroup_source_dir, '/output/include/resource.h']
# add_custom_command(['OUTPUT', resorce_h, 'COMMAND', 'echo', 'perl', perl_cmd, 'src/Resources.hpp', 'COMMAND', 'perl', perl_cmd, 'src/Resources.hpp', '>${RESORCE_H}', 'DEPENDS', 'src/Resources.hpp', 'output/include/dirstamp', 'WORKING_DIRECTORY', projectgroup_source_dir, 'COMMENT', '===='])
xcsoar_lib_type = 'static'
TARGET_NAME_lib = library(target_name, xcsoar_lib_type, c_files, header_files, resorce_h)
# add_custom_command(['TARGET', target_name, 'PRE_BUILD', 'COMMAND', cmake_command, '-E', 'make_directory', 'output/data', 'WORKING_DIRECTORY', projectgroup_source_dir, 'COMMENT', 'Make dir 'output/data''])
if '0'
  # Cmd-Line:
  # perl -ne "print \"#define $1 $2\n\" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;" src/Resources.hpp >output/include/resource.h
  perl_cmd = '-ne \"print \\\"\#define $1 $2\\n\\\" if /^MAKE_RESOURCE\\\(\(\\w+\), \(\\d+\)\\\)\;/\;\" '
  # add_custom_command(['TARGET', target_name, 'PRE_BUILD', 'COMMAND', 'echo', 'perl', perl_cmd, 'src/Resources.hpp', 'COMMAND', 'perl', perl_cmd, 'src/Resources.hpp', '>output/include/resource.h', 'DEPENDS', 'src/Resources.hpp', 'output/include/dirstamp', 'WORKING_DIRECTORY', projectgroup_source_dir, 'COMMENT', '===='])
endif
# set_target_properties(${TARGET_NAME}X PROPERTIES FOLDER Data)
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Data'])
