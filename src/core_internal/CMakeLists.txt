cmake_minimum_required(VERSION 3.16)

set(XMIPP4_CORE_INTERNAL_LIBRARY_TARGET "xmipp4-core-internal")

# Find all source and header files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c
)
file(
	GLOB_RECURSE 
	HEADERS
		${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

# Create the library
add_library(${XMIPP4_CORE_INTERNAL_LIBRARY_TARGET} STATIC ${SOURCES})
set_target_properties(
	${XMIPP4_CORE_INTERNAL_LIBRARY_TARGET} 
	PROPERTIES 
        POSITION_INDEPENDENT_CODE ON
)
target_include_directories(
    ${XMIPP4_CORE_INTERNAL_LIBRARY_TARGET}
	PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE 
		${PROJECT_SOURCE_DIR}/include
)
