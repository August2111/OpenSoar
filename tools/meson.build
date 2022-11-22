# message(['STATUS', '+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!'])
target_name = 'MathTables'
### add_custom_command( # TARGET ${TARGET_NAME} POST_BUILD
###         OUTPUT ${PROJECTGROUP_SOURCE_DIR}/output/include/MathTables.h
###         COMMENT "Create 'MathTables.h' with Sine tables"
###         echo ${TARGET_NAME} > MathTables.h 
###         COMMAND ${TARGET_NAME} > MathTables.h 
###         WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}/output/include
###         DEPENDS GenerateSineTables.cpp
###     )
TARGET_NAME_exe = executable(target_name, 'GenerateSineTables.cpp')
# set_target_properties([target_name, 'PROPERTIES', 'FOLDER', 'Data'])
