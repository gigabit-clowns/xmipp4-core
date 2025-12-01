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
		_xmipp4-core_detail
		Catch2::Catch2WithMain
		trompeloeil::trompeloeil
)

# HACK: Required for TROMPELOEIL_COUNT() to work with MSVC
if(MSVC)
	target_compile_options(
		${TESTS_NAME}
		PRIVATE
			/Zc:preprocessor 
	)
endif()

# Discover tests
catch_discover_tests(${TESTS_NAME})

# Install
install(
	TARGETS ${TESTS_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
