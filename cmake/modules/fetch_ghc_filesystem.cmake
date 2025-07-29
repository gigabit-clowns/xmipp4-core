cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_ghc_filesystem)
    set(options)
    set(oneValueArgs VERSION)
    set(multiValueArgs)
    cmake_parse_arguments(PARSE_ARGV 0 arg
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
    )

	FetchContent_Declare(
		ghc_filesystem
		GIT_REPOSITORY https://github.com/gulrak/filesystem.git
		GIT_TAG v${arg_VERSION}
	)

	FetchContent_MakeAvailable(ghc_filesystem)
endfunction()
