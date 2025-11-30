cmake_minimum_required(VERSION 3.16)

# Disable building tests/examples in dependencies
set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)        # Catch2: disable building its tests/benchmarks
set(TROMPELOEIL_BUILD_TESTING OFF CACHE BOOL "" FORCE)  # if supported by trompeloeil

include(CTest)
include(fetch_catch2)
include(fetch_trompeloeil)

fetch_catch2(VERSION 3.11.0)
fetch_trompeloeil(VERSION 49)

include(Catch)

add_subdirectory(unitary)
add_subdirectory(integration)
