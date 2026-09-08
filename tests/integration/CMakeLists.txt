# Add the test executable
add_executable(rexlib-integration-tests)
add_subdirectory(src)
target_link_libraries(
	rexlib-integration-tests
	PRIVATE
		rexlib
		rexlib-test-assets-interface
		Catch2::Catch2WithMain
)
add_dependencies(
	rexlib-integration-tests
	rexlib-test-assets
)

# On windows targets, copy the library to the test directory
if(WIN32)
	add_custom_command(TARGET rexlib-integration-tests
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rexlib> $<TARGET_FILE_DIR:rexlib-integration-tests>
	)
endif()

# Register tests
rexlib_register_tests(rexlib-integration-tests)
