include(FetchContent)

# Makes Eigen3::Eigen available, either from the system or by fetching it.
#
# There is no MINIMUM here, unlike the other dependencies. Eigen's config
# treats a version request as a major version match rather than a floor, so
# asking for the 3.3 every distribution carries rejects the 5.x this project
# is pinned to. Any Eigen providing Eigen3::Eigen is accepted instead.
#
# Eigen is a private dependency: it is linked PRIVATE and appears in no
# public header, so none of it belongs in an install of this project.
#
# Its CMakeLists installs its headers unconditionally - there is no option
# to turn that off - so adding it as a subdirectory would put (and overwrite)
# ~600 headers into <prefix>/include/eigen3. Populating the source without
# calling add_subdirectory() leaves its install rules out of this build
# entirely, which is what SOURCE_SUBDIR pointing at a directory that holds no
# CMakeLists achieves.
function(rexlib_add_eigen)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	if(REXLIB_USE_SYSTEM_EIGEN)
		find_package(Eigen3 REQUIRED NO_MODULE)
		return()
	endif()

	if(POLICY CMP0135)
		cmake_policy(SET CMP0135 NEW) # To avoid warnings
	endif()

	set(archive "https://gitlab.com/libeigen/eigen/-/archive")
	set(url "${archive}/${arg_VERSION}/eigen-${arg_VERSION}.tar.gz")
	FetchContent_Declare(eigen URL "${url}" SOURCE_SUBDIR do-not-build)
	FetchContent_MakeAvailable(eigen)

	# Guarded so that a superbuild in which another project already provides
	# Eigen does not trip over a second definition of the same target.
	if(NOT TARGET Eigen3::Eigen)
		add_library(eigen INTERFACE)
		add_library(Eigen3::Eigen ALIAS eigen)
		target_include_directories(eigen SYSTEM INTERFACE "${eigen_SOURCE_DIR}")
		target_compile_features(eigen INTERFACE cxx_std_14)
	endif()
endfunction()
