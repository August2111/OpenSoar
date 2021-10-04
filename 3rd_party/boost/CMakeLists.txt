# BOOST # BOOST # BOOST # BOOST # BOOST # BOOST # BOOST # BOOST # BOOST # BOOST 
cmake_minimum_required(VERSION 3.15)

message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
# set(TARGET_NAME "boost")
set(BOOST_VERSION "1.72.0")
set(XCSOAR_BOOST_VERSION "${TARGET_NAME}-${BOOST_VERSION}")  # reset!
set(BOOST_INSTALL_DIR "${LINK_LIBS}/${TARGET_NAME}/${XCSOAR_ZLIB_VERSION}")
set(BOOST_PREFIX "${EP_CMAKE}/${TARGET_NAME}/${XCSOAR_BOOST_VERSION}")

# set (BOOST_BUILD_COMMAND "bjam --with-regex toolset=gcc variant=debug link=static install --prefix=${CMAKE_CURRENT_BINARY_DIR}/boostinstall")

set (BOOST_BUILD_COMMAND "b2 --with-regex --with-json toolset=msvc variant=debug link=static install --prefix=${LINK_LIBS}/boost_test")
ExternalProject_Add (
    ${TARGET_NAME}
    PREFIX ${BOOST_PREFIX}
    GIT_REPOSITORY "https://github.com/boostorg/boost.git"
    GIT_TAG "boost-${BOOST_VERSION}"
    GIT_PROGRESS 1
    # UPDATE_COMMAND ${BOOST_BOOTSTRAP_CMD}
    UPDATE_COMMAND "bootstrap.bat"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${BOOST_BUILD_COMMAND}
    # BUILD_COMMAND ${BOOST_BUILD_CMD} stage
    INSTALL_COMMAND ""
    LOG_DOWNLOAD 1
    LOG_UPDATE 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
)


# ExternalProject_Add(
#     boost
#     PREFIX ${CMAKE_CURRENT_BINARY_DIR}/boost
#     GIT_REPOSITORY https://github.com/ryppl/boost-svn.git
#     GIT_TAG "Boost_1_41_0"
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND bjam --with-regex toolset=gcc variant=debug link=static install --prefix=${CMAKE_CURRENT_BINARY_DIR}/boostinstall
#     BUILD_IN_SOURCE 1
#     INSTALL_COMMAND ""
# )

