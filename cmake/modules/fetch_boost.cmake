cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_boost)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	set(GIT_TAG boost-${arg_VERSION})
	FetchContent_Declare(
		boost_assert
		GIT_REPOSITORY https://github.com/boostorg/assert.git
		GIT_TAG ${GIT_TAG}
	)
	FetchContent_Declare(
		boost_config
		GIT_REPOSITORY https://github.com/boostorg/config.git
		GIT_TAG ${GIT_TAG}
	)
	FetchContent_Declare(
		boost_core
		GIT_REPOSITORY https://github.com/boostorg/core.git
		GIT_TAG ${GIT_TAG}
	)
	FetchContent_Declare(
		boost_intrusive
		GIT_REPOSITORY https://github.com/boostorg/intrusive.git
		GIT_TAG ${GIT_TAG}
	)
	FetchContent_Declare(
		boost_move
		GIT_REPOSITORY https://github.com/boostorg/move.git
		GIT_TAG ${GIT_TAG}
	)
	FetchContent_Declare(
		boost_static_assert
		GIT_REPOSITORY https://github.com/boostorg/static_assert.git
		GIT_TAG ${GIT_TAG}
	)
	FetchContent_Declare(
		boost_container
		GIT_REPOSITORY https://github.com/boostorg/container.git
		GIT_TAG ${GIT_TAG}
	)

	set(BOOST_CONTAINER_HEADER_ONLY ON)
	FetchContent_MakeAvailable(
		boost_assert 
		boost_config 
		boost_core 
		boost_intrusive
		boost_move 
		boost_static_assert 
		boost_container
	)
endfunction()
