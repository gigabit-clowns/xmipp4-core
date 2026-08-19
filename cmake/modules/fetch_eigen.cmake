cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_eigen)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	FetchContent_Declare(
		eigen
		URL https://gitlab.com/libeigen/eigen/-/archive/${arg_VERSION}/eigen-${arg_VERSION}.tar.gz
	)

	FetchContent_MakeAvailable(eigen)
endfunction()
