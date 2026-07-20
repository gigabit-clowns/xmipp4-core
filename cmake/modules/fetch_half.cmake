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
	add_library(half INTERFACE)
	target_include_directories(
		half
		INTERFACE
			${half_INCLUDE_DIR}
	)

	# Disable literals as they are not used and they are producing whitespace
	# warnings in Clang. Remove when eventually gets fixed: 
	# https://sourceforge.net/p/half/discussion/general/thread/02d90668a5/
	target_compile_definitions(
		half
		INTERFACE
			HALF_ENABLE_CPP11_USER_LITERALS=0
	)
endfunction()
