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
		Catch2::Catch2WithMain
		trompeloeil::trompeloeil
)

# HACK: Required for TROMPELOEIL_COUNT() to work with MSVC
if(MSVC)
	target_compile_options(
		xmipp4-core-unitary-tests
		PRIVATE
			/Zc:preprocessor 
	)
endif()

# Discover tests
catch_discover_tests(xmipp4-core-unitary-tests)
