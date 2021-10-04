cmake_minimum_required(VERSION 3.9.3)

message(STATUS "## ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt")

set(TARGET_NAME libXCSoar)
# add_subdirectory(Dialogs)
include(CMakeSource.cmake)

if (${XCSOAR_LIB_TYPE} STREQUAL "WITHOUT")
  set(XCSOAR_LIB_TYPE STATIC)
endif()

# add_executable(${TARGET_NAME}
add_library(${TARGET_NAME} ${XCSOAR_LIB_TYPE}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SCRIPT_FILES}
)

# target_link_libraries(${TARGET_NAME} PUBLIC Screen)
# target_link_libraries(${TARGET_NAME} PUBLIC ${XCSOAR_LINK_LIBRARIES})
# target_link_libraries(${TARGET_NAME} PUBLIC Atmosphere Screen)

# set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".cti")
# set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${CMAKE_PROJECT_VERSION}${BO_PROJECT_VERSION_TWEAK})
# set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER _Main)

target_link_libraries(${TARGET_NAME} PUBLIC Dialogs Math)
add_dependencies(${TARGET_NAME} Dialogs Math)
