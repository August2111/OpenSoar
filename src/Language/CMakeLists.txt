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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
  source_group("Script Files" FILES  ${SCRIPT_FILES})

#  set_source_files_properties(${PROJECTGROUP_SOURCE_DIR}/temp/data/memory_fields.c PROPERTIES LANGUAGE C)
  else()
# TODO(aug): is this correct?
#  set_source_files_properties(${PROJECTGROUP_SOURCE_DIR}/temp/data/memory_fields.c PROPERTIES LANGUAGE C) # CXX)
endif()

### Translation Group
 # if (NOT UNIX)
set(MO_FILES)
set(MO_C_FILES)
if(1)  # Language creation moved to language.prj
        get_filename_component(PO_DIR  ${PROJECTGROUP_SOURCE_DIR}/po     ABSOLUTE)
        file(GLOB   LANGUAGE_PO_FILES   ${PO_DIR}/*.po) 
        get_filename_component(_python_dir ${PYTHON_APP} DIRECTORY)
        set(MSGFMT  "${_python_dir}/Tools/i18n/msgfmt.py")
        # set(MSGFMT  "${_python_dir}/Lib/site-packages/pythongettext/msgfmt.py")
        get_filename_component(MO_OUTDIR  ${OUTPUT}/data/mo     ABSOLUTE)
        get_filename_component(MO_C_OUTDIR  ${OUTPUT}/data/mo_c ABSOLUTE)
        foreach(translation ${LANGUAGE_PO_FILES})
          get_filename_component(lang_name ${translation} NAME_WE)
          # message(STATUS "+++ ${translation} ==> ${lang_name}")
          set(_mo_file ${MO_OUTDIR}/${lang_name}.mo)
          list(APPEND MO_FILES ${_mo_file})
          list(APPEND MO_C_FILES ${MO_C_OUTDIR}/${lang_name}.mo.c)
          set(TANSLATION_CMD  ${PYTHON_APP} ${MSGFMT} --output-file=${_mo_file} ${translation} )
          set(TANSLATION_CMD2 ${PYTHON_APP} ../../../tools/python/bin2c.py ${lang_name} ${MO_C_OUTDIR})
          # message(STATUS "+++ ${TANSLATION_CMD}   !!!!!!!!!!!!!!!!")
          add_custom_command(  # TARGET {TARGET_NAME} PRE_BUILD  ### ${lang_name}
               OUTPUT  ${_mo_file}
               COMMAND ${TANSLATION_CMD} 
               COMMAND ${TANSLATION_CMD2} 
               WORKING_DIRECTORY  ${MO_OUTDIR}
               DEPENDS ${translation}  
               COMMENT "${translation} ==> ${_mo_file}"
               # COMMENT "!!! ${TANSLATION_CMD} !!!"
          )
        endforeach()
endif()

if(MSVC)
    #view in project solution *.vcxproj
    source_group("Translation\\mo.c" FILES  ${MO_C_FILES})
    source_group("Translation\\mo" FILES  ${MO_FILES})
    source_group("Translation\\po" FILES  ${LANGUAGE_PO_FILES})
endif()
# endif()
# FindGettext...        
# # # GETTEXT_MSGMERGE_EXECUTABLE: the full path to the msgmerge tool.
# # # GETTEXT_MSGFMT_EXECUTABLE: the full path to the msgfmt tool.
# # # GETTEXT_FOUND: True if gettext has been found.
# # # GETTEXT_VERSION_STRING: the version of gettext found (since CMake 2.8.8)


add_library(${TARGET_NAME} ${XCSOAR_LIB_TYPE}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SCRIPT_FILES}
    ${LANGUAGE_PO_FILES}
    ${MO_FILES}
    ${MO_C_FILES}
#    ${PROJECTGROUP_SOURCE_DIR}/temp/data/memory_fields.c
    # _Deprecated files...
)

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD 
       COMMAND ${CMAKE_COMMAND} -E make_directory ${MO_OUTDIR}
       COMMAND ${CMAKE_COMMAND} -E make_directory ${MO_C_OUTDIR}
       COMMENT "make output directories"
)

target_link_libraries(${TARGET_NAME} PUBLIC Task system ) # _Deprecated)

# target_link_libraries( ${TARGET_NAME}
#     UTF8
# )
# message(FATAL_ERROR "Stop!")


set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Libs)
