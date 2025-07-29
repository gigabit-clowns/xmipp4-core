cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_half)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	set(NO_OP_COMMAND "${CMAKE_COMMAND} -E true")
	FetchContent_Declare(
		half
		URL https://sourceforge.net/projects/half/files/half/${arg_VERSION}/half-${arg_VERSION}.zip
		CONFIGURE_COMMAND ${NO_OP_COMMAND}
		BUILD_COMMAND ${NO_OP_COMMAND}
		INSTALL_COMMAND ${NO_OP_COMMAND}
	)
	FetchContent_MakeAvailable(half)

	set(half_INCLUDE_DIR ${half_SOURCE_DIR}/include)

	# Define the target
	set(HALF_TARGET "half")
	add_library(${HALF_TARGET} INTERFACE)
	target_include_directories(
		${HALF_TARGET}
		INTERFACE
			$<BUILD_INTERFACE:${half_INCLUDE_DIR}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)

	# Define install command
	install(
		FILES ${half_INCLUDE_DIR}/half.hpp 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
	install(
		TARGETS ${HALF_TARGET}
		EXPORT ${HALF_TARGET}-targets
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${HALF_TARGET}
	)
	install(
		EXPORT ${HALF_TARGET}-targets
		FILE ${HALF_TARGET}-config.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${HALF_TARGET}
	)

endfunction()
