cmake_minimum_required(VERSION 3.16)

add_subdirectory(assets)

# Register all source and header files
file(
	GLOB_RECURSE 
	TEST_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Add the test executable
add_executable(xmipp4-core-integration-tests ${TEST_SOURCES})
target_link_libraries(
	xmipp4-core-integration-tests
	PRIVATE 
		xmipp4-core
		Catch2::Catch2WithMain
)
add_dependencies(
	xmipp4-core-integration-tests
	xmipp4-core_integration_test_assets
)

# On windows targets, copy the library to the test directory
if(WIN32)
	add_custom_command(TARGET xmipp4-core-integration-tests
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:xmipp4-core> $<TARGET_FILE_DIR:xmipp4-core-integration-tests>
	)
endif()

# Discover tests
catch_discover_tests(xmipp4-core-integration-tests)
