include(FetchContent)

# Makes spdlog::spdlog_header_only available, either from the system or by
# fetching it. See rexlib_add_boost() on why VERSION and MINIMUM differ.
function(rexlib_add_spdlog)
	set(options)
	set(oneValueArgs VERSION MINIMUM)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(REXLIB_USE_SYSTEM_SPDLOG)
		find_package(spdlog ${arg_MINIMUM} REQUIRED CONFIG)
		return()
	endif()

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()

	# spdlog is linked header only and privately, so nothing of it belongs
	# in an install of this project.
	set(SPDLOG_INSTALL OFF)
	set(SPDLOG_BUILD_SHARED OFF)
	set(SPDLOG_BUILD_PIC ON)

	set(archive "https://github.com/gabime/spdlog/archive/refs/tags")
	FetchContent_Declare(spdlog URL "${archive}/v${arg_VERSION}.tar.gz")
	FetchContent_MakeAvailable(spdlog)
endfunction()
