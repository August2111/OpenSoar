# # # jetzt nicht mehr so ;-)  set(LIB_TARGET_NAME "Airspace")
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  set(Airspace_SOURCES
# # # jetzt nicht mehr so ;-)          Airspace/ActivePredicate.cpp
# # # jetzt nicht mehr so ;-)          Airspace/AirspaceComputerSettings.cpp
# # # jetzt nicht mehr so ;-)          Airspace/AirspaceGlue.cpp
# # # jetzt nicht mehr so ;-)          Airspace/AirspaceParser.cpp
# # # jetzt nicht mehr so ;-)          Airspace/AirspaceVisibility.cpp
# # # jetzt nicht mehr so ;-)          Airspace/NearestAirspace.cpp
# # # jetzt nicht mehr so ;-)          Airspace/ProtectedAirspaceWarningManager.cpp
# # # jetzt nicht mehr so ;-)  )

set(_SOURCES
        Airspace/ActivePredicate.cpp
        Airspace/AirspaceComputerSettings.cpp
        Airspace/AirspaceGlue.cpp
        Airspace/AirspaceParser.cpp
        Airspace/AirspaceVisibility.cpp
        Airspace/NearestAirspace.cpp
        Airspace/ProtectedAirspaceWarningManager.cpp
)

set(SCRIPT_FILES
    CMakeSource.cmake
)


# # # jetzt nicht mehr so ;-)  if (ON)
# # # jetzt nicht mehr so ;-)  #    add_subdirectory(src/Airspace)
# # # jetzt nicht mehr so ;-)  else()
# # # jetzt nicht mehr so ;-)  message(STATUS "+++ Start CMake ${CMAKE_CURRENT_LIST_DIR}!")  # ${CMAKE_SOURCE_DIR}/src/$LIB_NAME
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  set(SOURCE_FILES )
# # # jetzt nicht mehr so ;-)  foreach(source_file ${${LIB_TARGET_NAME}_SOURCES})
# # # jetzt nicht mehr so ;-)      string(REPLACE "${LIB_TARGET_NAME}/" "" source_file ${source_file})
# # # jetzt nicht mehr so ;-)  #    list(APPEND SOURCE_FILES ${source_file})
# # # jetzt nicht mehr so ;-)      if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${source_file})
# # # jetzt nicht mehr so ;-)         list(APPEND SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/${source_file})
# # # jetzt nicht mehr so ;-)      else()
# # # jetzt nicht mehr so ;-)          message(STATUS "+++ ${LIB_TARGET_NAME}/${source_file} don't exists!")
# # # jetzt nicht mehr so ;-)      endif()
# # # jetzt nicht mehr so ;-)      ### get_filename_component(src_path ${source_file} DIRECTORY)
# # # jetzt nicht mehr so ;-)      get_filename_component(src_path ${source_file} DIRECTORY)
# # # jetzt nicht mehr so ;-)      if (src_path)
# # # jetzt nicht mehr so ;-)          string(REPLACE "/" "\\" src_path ${src_path})
# # # jetzt nicht mehr so ;-)          source_group("Source" FILES  ${source_file})
# # # jetzt nicht mehr so ;-)      else()
# # # jetzt nicht mehr so ;-)          source_group("Source\\${src_path}" FILES  ${source_file})
# # # jetzt nicht mehr so ;-)      endif()
# # # jetzt nicht mehr so ;-)      message(STATUS "### '${src_path}'  --- ${source_file}")
# # # jetzt nicht mehr so ;-)  endforeach()
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  # include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# # # jetzt nicht mehr so ;-)  include_directories(${SRC}/${LIB_TARGET_NAME})
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  # message(STATUS "### ${LIB_TARGET_NAME} SOURCE_FILES:  ${SOURCE_FILES}")
# # # jetzt nicht mehr so ;-)  # message(FATAL_ERROR "### ${LIB_TARGET_NAME} SOURCE_FILES:  ${SOURCE_FILES}")
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  add_library(${LIB_TARGET_NAME} ${XCSOAR_LIB_TYPE}
# # # jetzt nicht mehr so ;-)      ${SOURCE_FILES}
# # # jetzt nicht mehr so ;-)      ${HEADER_FILES}
# # # jetzt nicht mehr so ;-)      ${CMAKE_CURRENT_LIST_DIR}/CMakeSource.cmake
# # # jetzt nicht mehr so ;-)      ${SCRIPT_FILES}
# # # jetzt nicht mehr so ;-)  )
# # # jetzt nicht mehr so ;-)  # message(FATAL_ERROR "Stop!")
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  set_target_properties(${LIB_TARGET_NAME} PROPERTIES FOLDER Libs)
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  target_link_libraries(${LIB_TARGET_NAME} PUBLIC IO)
# # # jetzt nicht mehr so ;-)  
# # # jetzt nicht mehr so ;-)  endif()
