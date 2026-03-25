cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_highway)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(HWY_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
	set(HWY_ENABLE_TESTS OFF CACHE BOOL "" FORCE)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	FetchContent_Declare(
		highway
		URL https://github.com/google/highway/archive/refs/tags/${arg_VERSION}.tar.gz
	)
	FetchContent_MakeAvailable(highway)
endfunction()
