cmake_minimum_required(VERSION 3.16)

include(CTest)
include(fetch_catch2)
include(fetch_trompeloeil)

fetch_catch2(VERSION 3.11.0)
fetch_trompeloeil(VERSION 49)

include(Catch)

add_subdirectory(unitary)
add_subdirectory(integration)
