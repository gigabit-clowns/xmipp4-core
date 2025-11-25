cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_catch2)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	FetchContent_Declare(
		Catch2
		URL https://github.com/catchorg/Catch2/archive/refs/tags/v${arg_VERSION}.tar.gz
	)
	FetchContent_MakeAvailable(Catch2)
	set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endfunction()
