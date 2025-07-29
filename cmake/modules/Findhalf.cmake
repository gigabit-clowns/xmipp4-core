cmake_minimum_required(VERSION 3.12)

include(FindPackageHandleStandardArgs)

# Try to find the half library include dir
find_path(
    half_INCLUDE_DIR 
    NAMES half.hpp
    PATH_SUFFIXES half_float half
)

# Parse version from header file
if(half_INCLUDE_DIR)
    file(
        STRINGS "${half_INCLUDE_DIR}/half.hpp"
        half_VERSION_LINE
        REGEX "Version"
    )
    string(REGEX MATCH "Version ([0-9]*\.[0-9]*\.[0-9]*)" _ ${half_VERSION_LINE})
    set(half_VERSION ${CMAKE_MATCH_1})
endif()

# Define the package variables
find_package_handle_standard_args(half 
    REQUIRED_VARS half_INCLUDE_DIR
    VERSION_VAR half_VERSION
)

# Define the target
set(HALF_TARGET "half")
add_library(${HALF_TARGET} INTERFACE IMPORTED)
target_include_directories(${HALF_TARGET} INTERFACE ${half_INCLUDE_DIR})
