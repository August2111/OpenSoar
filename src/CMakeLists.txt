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

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER _Main)

target_link_libraries(${TARGET_NAME} PUBLIC Dialogs Math) # Data)
# add_dependencies(${TARGET_NAME} Dialogs Math Data)
add_dependencies(${TARGET_NAME} Data)

if (1) # das ist neu amm 15.10.2021
# set (BMP_CONVERT_TOOL D:/Programs/ImageMagick/convert)
list(APPEND CMAKE_PROGRAM_PATH "D:/Programs/ImageMagick")
# find_program(BMP_CONVERT_TOOL D:/Programs/ImageMagick/convert REQUIRED)
find_program(BMP_CONVERT_TOOL NAMES convert REQUIRED)
message(STATUS "Image-App: '${BMP_CONVERT_TOOL}'")

# message(STATUS "### ### file(GLOB ICON_FILES  ${PROJECTGROUP_SOURCE_DIR}/Data/icons/*.svg)" )
file(GLOB ICON_FILES  ${PROJECTGROUP_SOURCE_DIR}/Data/icons/*.svg)
    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
           COMMENT Test
           COMMAND ${CMAKE_COMMAND} -E make_directory output/data/icons
           WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
    )
    foreach(icon_file ${ICON_FILES})
        string(REPLACE "${PROJECTGROUP_SOURCE_DIR}/Data/icons/" "" icon_file2 ${icon_file})
        string(REPLACE ".svg" ".bmp" bmp_file ${icon_file2})
        string(REPLACE ".svg" "_160.bmp" bmp160_file ${icon_file2})
        # # #     message(STATUS "### ### ${icon_file}  --- ${icon_file2} --- ${bmp_file} " )
        add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
           COMMENT ${bmp_file}
           COMMAND ${BMP_CONVERT_TOOL} ${icon_file}                +dither -compress none -type optimize -colors 256 bmp3:output/data/icons/${bmp_file}
           COMMAND ${BMP_CONVERT_TOOL} ${icon_file} -resize "160%%"  +dither -compress none -type optimize -colors 256 bmp3:output/data/icons/${bmp160_file}
           
           # DEPENDS ???
           WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
        )
    endforeach()

    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
           COMMENT Test
           COMMAND ${CMAKE_COMMAND} -E make_directory output/data/graphics
           # temp!!!            
           COMMAND ${BMP_CONVERT_TOOL} -size 160 Data/graphics/logo.svg bmp3:output/data/graphics/logo_160.bmp
           # COMMAND ${BMP_CONVERT_TOOL}  -size 160 -background white -layers flatten +matte +dither -compress none -type optimize -colors 256  Data/graphics/logo.svg bmp3:output/data/graphics/logo_160.bmp
           COMMAND ${BMP_CONVERT_TOOL}  -size 80 Data/graphics/logo.svg bmp3:output/data/graphics/logo_80.bmp
           # COMMAND ${BMP_CONVERT_TOOL}  -size 640 Data/graphics/launcher.svg output/data/graphics/logo_160.bmp
           COMMAND ${BMP_CONVERT_TOOL}  -background white -layers flatten +matte +dither -compress none -type optimize -colors 256  Data/graphics/launcher.svg bmp3:output/data/graphics/launcher_640.bmp
           COMMAND ${BMP_CONVERT_TOOL}  -crop "50%%x100%%" -scene 1  output/data/graphics/launcher_640.bmp "bmp3:output/data/graphics/launcher_640_%%d.bmp"
           # COMMAND ${BMP_CONVERT_TOOL}  -layers flatten +matte +dither -compress none -type optimize -colors 256 -crop 50%%x100%% -scene 1  Data/graphics/launcher.svg bmp3:output/data/graphics/launcher_640_2.bmp
           COMMAND ${BMP_CONVERT_TOOL}  Data/graphics/progress_border.svg bmp3:output/data/graphics/progress_border.bmp
           COMMAND ${BMP_CONVERT_TOOL}  -size 110x110 Data/graphics/title.svg bmp3:output/data/graphics/title_110.bmp
           # 
           COMMAND ${BMP_CONVERT_TOOL}  -size 320x320 Data/graphics/title.svg bmp3:output/data/graphics/title_320.bmp
           # COMMAND ${BMP_CONVERT_TOOL}  -size 320 -background white -layers flatten +matte +dither -compress none -type optimize -colors 256  Data/graphics/title.svg bmp3:output/data/graphics/title_320.bmp
           WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
    )


    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
       # COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECTGROUP_SOURCE_DIR}/output/include
       COMMAND ${CMAKE_COMMAND} -E make_directory output/include
       COMMAND perl tools/xcs2cpp.pl    Data/Status/default.xcs >output/include/Status_defaults.cpp
       DEPENDS Data/Status/default.xcs
       WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
)
endif()
