cmake_minimum_required(VERSION 3.15)
message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")

  get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

include(CMakeSource.cmake)
# organize the files in subdirectories
set(SOURCE_FILES )
foreach(source_file ${${TARGET_NAME}_SOURCES})
    string(REPLACE "${TARGET_NAME}/" "" source_file ${source_file})
    list(APPEND SOURCE_FILES ${source_file})
    get_filename_component(src_path ${source_file} DIRECTORY)
    if (src_path)
        string(REPLACE "/" "\\" src_path ${src_path})
    endif()
    source_group("Source\\${src_path}" FILES  ${source_file})
    # message(STATUS "### ${src_path}  --- ${source_file}")
endforeach()


if(NOT HEADER_FILES)  #  STREQUAL ""
file(GLOB_RECURSE   HEADER_FILES_TEMP   "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")  # ;../*.hxx;../*.h
#    message(FATAL_ERROR "### ### Header-Liste  --- ${HEADER_FILES_TEMP}" )
set(HEADER_FILES)
foreach(header_file ${HEADER_FILES_TEMP})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" header_file ${header_file})
    list(APPEND HEADER_FILES ${header_file})
    get_filename_component(src_path ${header_file} DIRECTORY)
    if (src_path)
        string(REPLACE "/" "\\" src_path ${src_path})
    endif()
    source_group("Header\\${src_path}" FILES  ${header_file})
    ## message(STATUS "### ### ${src_path}  --- ${header_file}" )
endforeach()
#     message(FATAL_ERROR "### ### Header-Liste  --- ${HEADER_FILES}" )
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME} ${XCSOAR_LIB_TYPE}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SCRIPT_FILES}
)
target_link_libraries(${TARGET_NAME} PUBLIC Cloud)

# message(FATAL_ERROR "Stop!")
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Libs)
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
       COMMAND ${CMAKE_COMMAND} -E make_directory output/include 

       COMMAND perl tools/Char2GCE.pl    src/Input/InputQueue.hpp >output/include/InputEvents_Char2GCE.cpp
       COMMAND perl tools/Char2NE.pl     src/Input/InputQueue.hpp >output/include/InputEvents_Char2NE.cpp
       COMMAND perl tools/Text2Event.pl  src/Input/InputEvents.hpp >output/include/InputEvents_Text2Event.cpp
       COMMAND perl tools/Text2GCE.pl    src/Input/InputQueue.hpp >output/include/InputEvents_Text2GCE.cpp
       COMMAND perl tools/Text2NE.pl     src/Input/InputQueue.hpp >output/include/InputEvents_Text2NE.cpp
       COMMAND perl tools/xci2cpp.pl     Data/Input/default.xci  >output/include/InputEvents_default.cpp
       DEPENDS src/Input/InputQueue.hpp src/Input/InputEvents.hpp Data/Input/default.xci
       WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
)
