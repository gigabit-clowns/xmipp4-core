cmake_minimum_required(VERSION 3.16)

# Test excecutable name
set(TESTS_NAME "${PROJECT_NAME}_unitary_tests")

# Register all source and header files
file(
	GLOB_RECURSE 
	TEST_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Add the test executable
add_executable(
    ${TESTS_NAME}
    ${TEST_SOURCES}
)
target_link_libraries(
	${TESTS_NAME} 
	PRIVATE 
		${PROJECT_NAME} 
		Catch2::Catch2WithMain
		trompeloeil::trompeloeil
)

# On windows targets, copy the library to the test directory
if(WIN32)
	add_custom_command(TARGET ${TESTS_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> $<TARGET_FILE_DIR:${TESTS_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:spdlog::spdlog> $<TARGET_FILE_DIR:${TESTS_NAME}>
	)
endif()

# Discover tests
catch_discover_tests(${TESTS_NAME})

# Install
install(
	TARGETS ${TESTS_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
