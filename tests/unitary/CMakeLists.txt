cmake_minimum_required(VERSION 3.16)

# Register all source and header files
file(
	GLOB_RECURSE 
	TEST_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Add the test executable
add_executable(
    xmipp4-core-unitary-tests
    ${TEST_SOURCES}
	$<TARGET_OBJECTS:xmipp4-core>
)
target_compile_definitions(
	xmipp4-core-unitary-tests
	PRIVATE
		XMIPP4_CORE_NO_EXPORTS
)
target_include_directories(
	xmipp4-core-unitary-tests
	PRIVATE 
		${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(
	xmipp4-core-unitary-tests
	PRIVATE
		xmipp4-core
		$<TARGET_PROPERTY:xmipp4-core,LINK_LIBRARIES>
		xmipp4-test-assets-interface
		Catch2::Catch2WithMain
		trompeloeil::trompeloeil
)
add_dependencies(
	xmipp4-core-unitary-tests
	xmipp4-test-assets
)

if(MSVC)
	target_compile_options(
		xmipp4-core-unitary-tests
		PRIVATE
			/Zc:preprocessor # HACK: Required for TROMPELOEIL_COUNT()
			/bigobj # Required for test_type_and_inner_stride_dispatch.cpp
	)
endif()

# On Windows the plugin DLLs that some tests load (e.g. dynamic_library) depend
# on xmipp4-core.dll. The loader resolves a plugin's imports from the test
# executable's directory, so copy the library next to it. Unlike the
# integration suite this executable embeds the core objects and never imports
# the DLL itself, hence the DLL would otherwise not be present here.
if(WIN32)
	add_custom_command(TARGET xmipp4-core-unitary-tests
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:xmipp4-core> $<TARGET_FILE_DIR:xmipp4-core-unitary-tests>
	)
endif()

# Discover tests
catch_discover_tests(xmipp4-core-unitary-tests)
