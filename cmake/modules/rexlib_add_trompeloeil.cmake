include(FetchContent)

# Makes trompeloeil::trompeloeil available, either from the system or by
# fetching it. See rexlib_add_boost() on why VERSION and MINIMUM differ.
function(rexlib_add_trompeloeil)
	set(options)
	set(oneValueArgs VERSION MINIMUM)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(REXLIB_USE_SYSTEM_TROMPELOEIL)
		find_package(trompeloeil ${arg_MINIMUM} REQUIRED CONFIG)
		return()
	endif()

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()

	set(archive "https://github.com/rollbear/trompeloeil/archive/refs/tags")
	FetchContent_Declare(Trompeloeil URL "${archive}/v${arg_VERSION}.tar.gz")
	FetchContent_MakeAvailable(Trompeloeil)
endfunction()
