cmake_minimum_required(VERSION 3.15)
  
set(TARGET_NAME Scripts)

file(GLOB_RECURSE SCRIPT_FILES
    ${PROJECTGROUP_SOURCE_DIR}/src/*.txt
    ${PROJECTGROUP_SOURCE_DIR}/src/*.cmake
    ${PROJECTGROUP_SOURCE_DIR}/build/*.txt
    ${PROJECTGROUP_SOURCE_DIR}/src/*.in
)
list(APPEND 
  ${PROJECTGROUP_SOURCE_DIR}/CMakeSource.cmake
  ${PROJECTGROUP_SOURCE_DIR}/NEWS.txt
)

file(GLOB_RECURSE MAKE_FILES 
    ${PROJECTGROUP_SOURCE_DIR}/build/*.py 
    ${PROJECTGROUP_SOURCE_DIR}/build/*.mk
)

include(xcsoar.cmake)

set(_subfolder Scripts)
set(_replacement ${PROJECTGROUP_SOURCE_DIR})

foreach(file ${SCRIPT_FILES})
    string(REPLACE "${_replacement}/" "" file ${file})
    get_filename_component(src_path ${file} DIRECTORY)
    if (src_path)
        string(REPLACE "/" "\\" src_path ${src_path})
        set(_folder_name "${_subfolder}\\{src_path}")
    else()
        set(_folder_name "${_subfolder}")
    endif()
    source_group(${_folder_name} FILES  ${file})
    
    message(STATUS "### ${src_path}  --- ${file}")
endforeach()


add_custom_target(${TARGET_NAME}
    EXCLUDE_FROM_ALL
    ${SCRIPT_FILES}
    ${MAKE_FILES}
)


set_target_properties(${TARGET_NAME} PROPERTIES FOLDER _Scripts)
