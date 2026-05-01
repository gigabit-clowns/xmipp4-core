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
		C_VISIBILITY_PRESET hidden
    	CXX_VISIBILITY_PRESET hidden
)
target_include_directories(
	xmipp4-core
	PUBLIC 
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
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
	PRIVATE
		spdlog::spdlog_header_only
		Boost::filesystem # Eventually replace by std::filesystem (C++17)
		Boost::container # Eventually replace in C++23
		Boost::unordered # Eventually replace in C++23
		Boost::intrusive
		fmt::fmt
		half
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

if(XMIPP4_CORE_ENABLE_RUST_BRIDGE)
	set(XMIPP4_RUST_CORE_TYPES_MANIFEST ${PROJECT_SOURCE_DIR}/rust/core-types/Cargo.toml)
	set(XMIPP4_RUST_CORE_TYPES_CARGO_ARGS --manifest-path ${XMIPP4_RUST_CORE_TYPES_MANIFEST} --release)
	set(XMIPP4_RUST_CORE_TYPES_LIB_DIR ${PROJECT_SOURCE_DIR}/rust/target/release)
	set(XMIPP4_RUST_CORE_TYPES_PREBUILD_COMMAND ${CMAKE_COMMAND} -E true)

	if(WIN32)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			list(APPEND XMIPP4_RUST_CORE_TYPES_CARGO_ARGS --target x86_64-pc-windows-gnu)
			set(XMIPP4_RUST_CORE_TYPES_PREBUILD_COMMAND rustup target add x86_64-pc-windows-gnu)
			set(XMIPP4_RUST_CORE_TYPES_LIB_DIR ${PROJECT_SOURCE_DIR}/rust/target/x86_64-pc-windows-gnu/release)
			set(XMIPP4_RUST_CORE_TYPES_LIB ${XMIPP4_RUST_CORE_TYPES_LIB_DIR}/libxmipp4_core_types.a)
		else()
			set(XMIPP4_RUST_CORE_TYPES_LIB ${XMIPP4_RUST_CORE_TYPES_LIB_DIR}/xmipp4_core_types.lib)
		endif()
	else()
		set(XMIPP4_RUST_CORE_TYPES_LIB ${XMIPP4_RUST_CORE_TYPES_LIB_DIR}/libxmipp4_core_types.a)
	endif()

	add_custom_command(
		OUTPUT ${XMIPP4_RUST_CORE_TYPES_LIB}
		COMMAND ${XMIPP4_RUST_CORE_TYPES_PREBUILD_COMMAND}
		COMMAND cargo build ${XMIPP4_RUST_CORE_TYPES_CARGO_ARGS}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/rust
		COMMENT "Building Rust bridge static library (xmipp4-core-types)"
		VERBATIM
	)

	add_custom_target(
		xmipp4-rust-core-types-build
		DEPENDS ${XMIPP4_RUST_CORE_TYPES_LIB}
	)

	add_library(xmipp4-rust-core-types STATIC IMPORTED GLOBAL)
	set_target_properties(
		xmipp4-rust-core-types
		PROPERTIES
			IMPORTED_LOCATION ${XMIPP4_RUST_CORE_TYPES_LIB}
	)

	add_dependencies(xmipp4-core xmipp4-rust-core-types-build)
	target_link_libraries(
		xmipp4-core
		PRIVATE
			xmipp4-rust-core-types
	)
	if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_link_libraries(
			xmipp4-core
			PRIVATE
				ws2_32
				ntdll
				userenv
		)
	endif()
	target_compile_definitions(
		xmipp4-core
		PUBLIC
			XMIPP4_CORE_ENABLE_RUST_BRIDGE=1
	)
endif()

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
