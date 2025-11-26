cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_trompeloeil)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	FetchContent_Declare(
		Trompeloeil
		URL https://github.com/rollbear/trompeloeil/archive/refs/tags/v${arg_VERSION}.tar.gz
	)
	FetchContent_MakeAvailable(Trompeloeil)
endfunction()
