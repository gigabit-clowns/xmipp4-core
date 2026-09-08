include(FetchContent)

# Makes pocketfft::pocketfft available, either from the system or by fetching
# it. Pinned by COMMIT, as pocketfft makes no releases.
#
# pocketfft ships no CMake of its own, so the system path goes through this
# project's Findpocketfft.cmake. That module cannot check a version, which is
# why this function takes no MINIMUM.
function(rexlib_add_pocketfft)
	set(options)
	set(oneValueArgs COMMIT)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(REXLIB_USE_SYSTEM_POCKETFFT)
		find_package(pocketfft REQUIRED MODULE)
		return()
	endif()

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()

	set(archive "https://github.com/mreineck/pocketfft/archive")
	FetchContent_Declare(pocketfft URL "${archive}/${arg_COMMIT}.tar.gz")
	FetchContent_MakeAvailable(pocketfft)

	if(NOT TARGET pocketfft::pocketfft)
		add_library(pocketfft INTERFACE)
		add_library(pocketfft::pocketfft ALIAS pocketfft)
		target_include_directories(
			pocketfft
			SYSTEM INTERFACE
				${pocketfft_SOURCE_DIR}
		)
	endif()
endfunction()
