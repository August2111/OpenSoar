cmake_minimum_required(VERSION 3.15)
message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")

set(TARGET_NAME MathTables)

add_executable(${TARGET_NAME} GenerateSineTables.cpp)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Tools)


add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMENT "Create 'MathTables.h' with Sine tables"
        COMMAND ${TARGET_NAME} > MathTables.h 
        WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}/output/include
        DEPENDS GenerateSineTables.cpp
    )
