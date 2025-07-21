cmake_minimum_required(VERSION 3.16)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/faulty_plugin2.cpp)
add_library(faulty_plugin2 MODULE ${SOURCES})
target_link_libraries(
	faulty_plugin2
	PRIVATE
		xmipp4-core
)
