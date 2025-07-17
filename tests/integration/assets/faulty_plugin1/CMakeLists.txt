cmake_minimum_required(VERSION 3.16)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/faulty_plugin1.cpp)
add_library(faulty_plugin1 MODULE ${SOURCES})
set_target_properties(
	faulty_plugin1
	PROPERTIES 
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_FAULTY_PLUGIN1_EXPORTING"
)
target_link_libraries(
	faulty_plugin1
	PRIVATE
		xmipp4-core
)
