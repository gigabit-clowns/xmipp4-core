cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_libfmt)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	FetchContent_Declare(
		libfmt
		URL https://github.com/fmtlib/fmt/archive/refs/tags/${arg_VERSION}.tar.gz
	)
	set(DCMAKE_POSITION_INDEPENDENT_CODE ON)
	FetchContent_MakeAvailable(libfmt)
endfunction()
