fs = import('fs')

target_name = 'Scripts'
# file(['GLOB_RECURSE', 'SCRIPT_FILES', projectgroup_source_dir, '/src/*.txt', projectgroup_source_dir, '/src/*.cmake', projectgroup_source_dir, '/build/*.txt', projectgroup_source_dir, '/src/*.in'])
# list(['APPEND', projectgroup_source_dir, '/CMakeSource.cmake', projectgroup_source_dir, '/NEWS.txt'])
# file(['GLOB_RECURSE', 'MAKE_FILES', projectgroup_source_dir, '/build/*.py', projectgroup_source_dir, '/build/*.mk'])
_subfolder = 'Scripts'
_replacement = projectgroup_source_dir
# foreach(['file', script_files])
# string(['REPLACE', '${_replacement}/', '', 'file', file])
# get_filename_component(['src_path', file, 'DIRECTORY'])
### src_path = fs.path('D:/Projects/XCSoar') # projectgroup_source_dir
# src_path = fs.path('D:/Projects/XCSoar') # projectgroup_source_dir
src_path = 'D:/Projects/XCSoar' # projectgroup_source_dir
if fs.is_dir(src_path)
  # string(['REPLACE', '/', '\\', 'src_path', src_path])
  _folder_name = '${_subfolder}\\{src_path}'
else
  _folder_name = '${_subfolder}'
endif
# source_group([_folder_name, 'FILES', file])
# message(['STATUS', '### ${src_path}  --- ${file}'])
# endforeach()
# add_custom_target([target_name, 'EXCLUDE_FROM_ALL', script_files, make_files])
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', '_Scripts'])
