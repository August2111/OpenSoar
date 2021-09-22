# message(['STATUS', '## ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt'])
target_name = 'libXCSoar'
# add_subdirectory(Dialogs)
if xcsoar_lib_type, 'STREQUAL', 'WITHOUT'
  xcsoar_lib_type = 'STATIC'
endif
# add_executable(${TARGET_NAME}
TARGET_NAME_lib = library(target_name, xcsoar_lib_type, source_files, header_files, script_files)
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', '_Main'])
# target_link_libraries([target_name, 'PUBLIC', 'Dialogs', 'Math'])
# Data)
# add_dependencies(${TARGET_NAME} Dialogs Math Data)
# add_dependencies([target_name, 'Data'])
if '1'
  # das ist neu amm 15.10.2021
  # set (BMP_CONVERT_TOOL D:/Programs/ImageMagick/convert)
  # list(['APPEND', 'CMAKE_PROGRAM_PATH', 'D:/Programs/ImageMagick'])
  # find_program(BMP_CONVERT_TOOL D:/Programs/ImageMagick/convert REQUIRED)
  # find_program(['BMP_CONVERT_TOOL', 'NAMES', 'convert', 'REQUIRED'])
  # message(['STATUS', 'Image-App: '${BMP_CONVERT_TOOL}''])
  # message(STATUS "### ### file(GLOB ICON_FILES  ${PROJECTGROUP_SOURCE_DIR}/Data/icons/*.svg)" )
  # add_custom_command(['TARGET', target_name, 'PRE_BUILD', 'COMMENT', 'Test', 'COMMAND', cmake_command, '-E', 'make_directory', 'output/data/icons', 'WORKING_DIRECTORY', projectgroup_source_dir])
  # foreach(['icon_file', icon_files])
  # string(['REPLACE', '${PROJECTGROUP_SOURCE_DIR}/Data/icons/', '', 'icon_file2', icon_file])
  # string(['REPLACE', '.svg', '.bmp', 'bmp_file', icon_file2])
  # string(['REPLACE', '.svg', '_160.bmp', 'bmp160_file', icon_file2])
  # # #     message(STATUS "### ### ${icon_file}  --- ${icon_file2} --- ${bmp_file} " )
  # add_custom_command(['TARGET', target_name, 'PRE_BUILD', 'COMMENT', bmp_file, 'COMMAND', bmp_convert_tool, icon_file, '+dither', '-compress', 'none', '-type', 'optimize', '-colors', '256', 'bmp3:output/data/icons/${bmp_file}', 'COMMAND', bmp_convert_tool, icon_file, '-resize', '160%%', '+dither', '-compress', 'none', '-type', 'optimize', '-colors', '256', 'bmp3:output/data/icons/${bmp160_file}', 'WORKING_DIRECTORY', projectgroup_source_dir])
  # endforeach()
  # add_custom_command(['TARGET', target_name, 'PRE_BUILD', 'COMMENT', 'Test', 'COMMAND', cmake_command, '-E', 'make_directory', 'output/data/graphics', 'COMMAND', bmp_convert_tool, '-size', '160', 'Data/graphics/logo.svg', 'bmp3:output/data/graphics/logo_160.bmp', 'COMMAND', bmp_convert_tool, '-size', '80', 'Data/graphics/logo.svg', 'bmp3:output/data/graphics/logo_80.bmp', 'COMMAND', bmp_convert_tool, '-background', 'white', '-layers', 'flatten', '+matte', '+dither', '-compress', 'none', '-type', 'optimize', '-colors', '256', 'Data/graphics/launcher.svg', 'bmp3:output/data/graphics/launcher_640.bmp', 'COMMAND', bmp_convert_tool, '-crop', '50%%x100%%', '-scene', '1', 'output/data/graphics/launcher_640.bmp', 'bmp3:output/data/graphics/launcher_640_%%d.bmp', 'COMMAND', bmp_convert_tool, 'Data/graphics/progress_border.svg', 'bmp3:output/data/graphics/progress_border.bmp', 'COMMAND', bmp_convert_tool, '-size', '110x110', 'Data/graphics/title.svg', 'bmp3:output/data/graphics/title_110.bmp', 'COMMAND', bmp_convert_tool, '-size', '320x320', 'Data/graphics/title.svg', 'bmp3:output/data/graphics/title_320.bmp', 'WORKING_DIRECTORY', projectgroup_source_dir])
  # add_custom_command(['TARGET', target_name, 'PRE_BUILD', 'COMMAND', cmake_command, '-E', 'make_directory', 'output/include', 'COMMAND', 'perl', 'tools/xcs2cpp.pl', 'Data/Status/default.xcs', '>output/include/Status_defaults.cpp', 'DEPENDS', 'Data/Status/default.xcs', 'WORKING_DIRECTORY', projectgroup_source_dir])
endif
