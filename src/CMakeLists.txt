cmake_minimum_required(VERSION 3.16)

set(XMIPP4_CORE_LIBRARY_TARGET "xmipp4-core")
set(XMIPP4_CORE_DETAIL_LIBRARY_TARGET "_xmipp4-core_detail")

# Find all source and header files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# Create the shared library
add_library(${XMIPP4_CORE_LIBRARY_TARGET} SHARED ${SOURCES})
set_target_properties(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PROPERTIES 
		SOVERSION ${PROJECT_VERSION}
		DEFINE_SYMBOL "XMIPP4_CORE_EXPORTING"
)
target_include_directories(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PUBLIC 
	  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PRIVATE
		VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
		VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
		VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)

# Link dependencies
if(WIN32)
	target_link_libraries(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE ws2_32 # Winsock
	) 
endif()

target_link_libraries(
	${XMIPP4_CORE_LIBRARY_TARGET} 
	PUBLIC
		Threads::Threads
		spdlog::spdlog_header_only
		half
	PRIVATE
		ghc_filesystem # Eventually replace by std::filesystem (C++17)
		Boost::container # Eventually replace in C++23
		${CMAKE_DL_LIBS}
)

# Enable all warnings during compilation
if(MSVC)
	target_compile_options(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE 
			/W4
			/wd4996 # getenv warning
	)
else()
	target_compile_options(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PRIVATE 
			-Wall 
			-Wextra 
			-Wpedantic
	)
endif()

# Disable min/max macros in windows
if(MSVC)
	target_compile_definitions(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PUBLIC 
			NOMINMAX
	)
endif()

# Disable builtin functions in MinGW
if(MINGW)
	target_compile_definitions(
		${XMIPP4_CORE_LIBRARY_TARGET} 
		PUBLIC 
			XMIPP4_NO_BUILTIN
	)
endif()

# Install library's binary files and headers
install(
	TARGETS ${XMIPP4_CORE_LIBRARY_TARGET}
	EXPORT ${XMIPP4_CORE_LIBRARY_TARGET}-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create and install CMake config files
if(WIN32)
	set(XMIPP4_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR}/xmipp4-plugins)
else()
	set(XMIPP4_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/xmipp4-plugins)
endif()
set(XMIPP4_PLUGIN_INSTALL_DIR ${XMIPP4_PLUGIN_INSTALL_DIR} CACHE PATH "Directory where the xmipp4 plugins are installed")

configure_package_config_file(
  	${PROJECT_SOURCE_DIR}/cmake/config/config.cmake.in
  	${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config.cmake
  	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config-version.cmake
	COMPATIBILITY SameMinorVersion
)
install(
  	EXPORT ${XMIPP4_CORE_LIBRARY_TARGET}-targets
  	FILE ${XMIPP4_CORE_LIBRARY_TARGET}-targets.cmake
  	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}
)
install(
  	FILES 
		${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${XMIPP4_CORE_LIBRARY_TARGET}-config-version.cmake
  	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${XMIPP4_CORE_LIBRARY_TARGET}
)

# Create the detail target
add_library(${XMIPP4_CORE_DETAIL_LIBRARY_TARGET} INTERFACE)
target_sources(
	${XMIPP4_CORE_DETAIL_LIBRARY_TARGET} 
	INTERFACE 
		$<TARGET_OBJECTS:${XMIPP4_CORE_LIBRARY_TARGET}>
)
target_link_libraries(
	${XMIPP4_CORE_DETAIL_LIBRARY_TARGET}  
	INTERFACE 
		$<TARGET_PROPERTY:${XMIPP4_CORE_LIBRARY_TARGET},LINK_LIBRARIES>
)
target_include_directories(
	${XMIPP4_CORE_DETAIL_LIBRARY_TARGET} 
	INTERFACE
		$<TARGET_PROPERTY:${XMIPP4_CORE_LIBRARY_TARGET},INCLUDE_DIRECTORIES>
    	${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(
	${XMIPP4_CORE_DETAIL_LIBRARY_TARGET} 
	INTERFACE
		$<TARGET_PROPERTY:${XMIPP4_CORE_LIBRARY_TARGET},INTERFACE_COMPILE_DEFINITIONS>
		"XMIPP4_CORE_NO_EXPORTS"
)
