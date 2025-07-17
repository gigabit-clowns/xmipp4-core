cmake_minimum_required(VERSION 3.16)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/dummy_plugin.cpp)
add_library(dummy_plugin MODULE ${SOURCES})
set_target_properties(
	dummy_plugin
	PROPERTIES 
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_DUMMY_PLUGIN_EXPORTING"
)
target_link_libraries(
	dummy_plugin
	PRIVATE
		xmipp4-core
)
