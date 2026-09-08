cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_half)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()
	FetchContent_Declare(
		half
		URL https://sourceforge.net/projects/half/files/half/${arg_VERSION}/half-${arg_VERSION}.zip
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
