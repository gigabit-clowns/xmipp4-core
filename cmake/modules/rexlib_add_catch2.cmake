include(FetchContent)

# Makes Catch2::Catch2WithMain and Catch2's own Catch module available, either
# from the system or by fetching it. See rexlib_add_boost() on why VERSION and
# MINIMUM differ.
function(rexlib_add_catch2)
	set(options)
	set(oneValueArgs VERSION MINIMUM)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(REXLIB_USE_SYSTEM_CATCH2)
		find_package(Catch2 ${arg_MINIMUM} REQUIRED CONFIG)
	else()
		if(POLICY CMP0135)
			cmake_policy(SET CMP0135 NEW) # To avoid warnings
		endif()

		set(archive "https://github.com/catchorg/Catch2/archive/refs/tags")
		FetchContent_Declare(Catch2 URL "${archive}/v${arg_VERSION}.tar.gz")
		FetchContent_MakeAvailable(Catch2)
	endif()

	# Both paths append the directory holding Catch.cmake, which provides
	# catch_discover_tests(), to CMAKE_MODULE_PATH. That happens in this
	# function's scope, so it has to be handed back for the caller's
	# include(Catch) to find it.
	set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endfunction()
