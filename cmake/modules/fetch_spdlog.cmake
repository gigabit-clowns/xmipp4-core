cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_spdlog)
    set(options)
    set(oneValueArgs VERSION)
    set(multiValueArgs)
    cmake_parse_arguments(PARSE_ARGV 0 arg
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
    )

    set(SPDLOG_INSTALL ON)
	set(SPDLOG_BUILD_SHARED ON)
	FetchContent_Declare(
		spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG v${arg_VERSION}
	)

	FetchContent_MakeAvailable(spdlog)
endfunction()
