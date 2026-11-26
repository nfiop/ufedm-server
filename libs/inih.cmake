cmake_minimum_required(VERSION 3.20)

include(ExternalProject)

set(INIH_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/inih-src")

ExternalProject_Add(inih_external
    GIT_REPOSITORY https://github.com/benhoyt/inih.git
    GIT_TAG r56
    GIT_SHALLOW TRUE

    SOURCE_DIR "${INIH_SOURCE_DIR}"

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""

    UPDATE_DISCONNECTED TRUE
)

# ExternalProject downloads at build time, so tell CMake
# that this source will be generated.
set_source_files_properties(
    "${INIH_SOURCE_DIR}/ini.c"
    PROPERTIES GENERATED TRUE
)

# Build inih as a regular static library.
add_library(inih STATIC
    "${INIH_SOURCE_DIR}/ini.c"
)

add_dependencies(inih inih_external)

target_include_directories(inih PUBLIC
    "${INIH_SOURCE_DIR}"
)

# Parser implementation configuration.
target_compile_definitions(inih PRIVATE
    INI_USE_STACK=0
    INI_ALLOW_REALLOC=1
    INI_INITIAL_ALLOC=256
    INI_MAX_LINE=4096
    INI_MAX_SECTION=128
    INI_MAX_NAME=128
    INI_STOP_ON_FIRST_ERROR=1
)

# This definition must also be visible to consumers,
# because it changes the ini_handler function signature.
target_compile_definitions(inih PUBLIC
    INI_HANDLER_LINENO=1
)
