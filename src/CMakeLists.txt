cmake_minimum_required(VERSION 3.16)

# Find all source files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# Define the target
add_library(xmipp4-core SHARED ${SOURCES})
set_target_properties(
	xmipp4-core
	PROPERTIES 
		SOVERSION ${PROJECT_VERSION}
		DEFINE_SYMBOL XMIPP4_CORE_EXPORTING
)
target_include_directories(
	xmipp4-core
	PUBLIC 
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(
	xmipp4-core
	PRIVATE
		VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
		VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
		VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)

if(MSVC)
	target_compile_definitions(
		xmipp4-core
		PUBLIC
			NOMINMAX
	)
endif()

if(MSVC)
	target_compile_options(
		xmipp4-core
		PRIVATE 
			/W4
			/wd4996 # getenv warning
	)
else()
	target_compile_options(
		xmipp4-core
		PRIVATE 
			-Wall 
			-Wextra 
			-Wpedantic
	)
endif()

# Add coverage flags if requested
if(XMIPP4_CORE_ENABLE_COVERAGE)
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		target_compile_options(xmipp4-core PRIVATE --coverage -O0 -g)
		target_link_options(xmipp4-core PUBLIC --coverage)
	elseif(MSVC)
		target_compile_options(xmipp4-core PRIVATE /Z7 /Od)
		target_link_options(xmipp4-core PUBLIC /DEBUG)
	endif()
endif()

# Link dependencies
target_link_libraries(
	xmipp4-core
	PUBLIC
		Threads::Threads
		spdlog::spdlog_header_only
		half
	PRIVATE
		Boost::filesystem # Eventually replace by std::filesystem (C++17)
		Boost::container # Eventually replace in C++23
		Boost::unordered # Eventually replace in C++23
		Boost::intrusive
		${CMAKE_DL_LIBS}
)

if(WIN32)
	target_link_libraries(
		xmipp4-core
		PRIVATE 
			ws2_32 # Winsock
	)
endif()

# Create the alias
add_library(xmipp4::core ALIAS xmipp4-core)

# Install library's binary files and headers
install(
	TARGETS xmipp4-core
	EXPORT xmipp4-core-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create and install CMake config files
configure_package_config_file(
		${PROJECT_SOURCE_DIR}/cmake/config/config.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/xmipp4-core-config.cmake
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xmipp4-core
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/xmipp4-core-config-version.cmake
	COMPATIBILITY SameMinorVersion
)
install(
	EXPORT xmipp4-core-targets
	FILE xmipp4-core-targets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xmipp4-core
)
install(
	FILES 
	${CMAKE_CURRENT_BINARY_DIR}/xmipp4-core-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/xmipp4-core-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xmipp4-core
)
