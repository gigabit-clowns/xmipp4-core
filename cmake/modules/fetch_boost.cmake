cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_boost)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()
	FetchContent_Declare(
		Boost
		URL https://github.com/boostorg/boost/releases/download/boost-${arg_VERSION}/boost-${arg_VERSION}-cmake.tar.gz
	)

	# Only the modules the sources include. Every one listed here is built,
	# so an unused entry is paid for on every clean build.
	set(BOOST_INCLUDE_LIBRARIES
		container
		container_hash
		filesystem
		interprocess
	)
	set(BOOST_CONTAINER_HEADER_ONLY ON)
	set(BOOST_USE_STATIC_LIBS ON)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	FetchContent_MakeAvailable(Boost)

endfunction()
