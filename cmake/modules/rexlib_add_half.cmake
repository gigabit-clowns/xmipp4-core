include(FetchContent)

# Makes half::half available, either from the system or by fetching it.
#
# half ships no CMake package config of its own, so the system path goes
# through this project's Findhalf.cmake. That module cannot check a version,
# which is why this function takes no MINIMUM.
function(rexlib_add_half)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(REXLIB_USE_SYSTEM_HALF)
		find_package(half REQUIRED MODULE)
		return()
	endif()

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()

	set(archive "https://sourceforge.net/projects/half/files/half")
	set(url "${archive}/${arg_VERSION}/half-${arg_VERSION}.zip")
	FetchContent_Declare(half URL "${url}")
	FetchContent_MakeAvailable(half)

	if(NOT TARGET half::half)
		add_library(half INTERFACE)
		add_library(half::half ALIAS half)
		target_include_directories(
			half
			SYSTEM INTERFACE
				${half_SOURCE_DIR}/include
		)

		# Disable literals as they are not used and they are producing
		# whitespace warnings in Clang. Remove when eventually gets fixed:
		# https://sourceforge.net/p/half/discussion/general/thread/02d90668a5/
		target_compile_definitions(
			half
			INTERFACE
				HALF_ENABLE_CPP11_USER_LITERALS=0
		)
	endif()
endfunction()
