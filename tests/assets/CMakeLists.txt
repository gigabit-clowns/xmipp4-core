# Build the mock plugins consumed by the tests as runtime assets.
add_subdirectory(dummy_plugin)
add_subdirectory(faulty_plugin1)
add_subdirectory(faulty_plugin2)

# Single directory where every asset is staged. Both the unitary and the
# integration test suites resolve their assets from here through an absolute
# path, so the tests no longer depend on their working directory.
set(XMIPP4_TEST_ASSET_ROOT ${CMAKE_CURRENT_BINARY_DIR}/assets)
set(XMIPP4_TEST_PLUGINS_DIRECTORY ${XMIPP4_TEST_ASSET_ROOT}/plugins)

# Stage the assets next to the build tree.
add_custom_target(
	xmipp4-test-assets
	COMMAND ${CMAKE_COMMAND} -E make_directory ${XMIPP4_TEST_ASSET_ROOT}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lorem_ipsum.txt ${XMIPP4_TEST_ASSET_ROOT}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${XMIPP4_TEST_PLUGINS_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dummy-plugin> ${XMIPP4_TEST_PLUGINS_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:faulty-plugin1> ${XMIPP4_TEST_PLUGINS_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:faulty-plugin2> ${XMIPP4_TEST_PLUGINS_DIRECTORY}
)

# Interface target that gives any test suite access to the assets: the
# assets.hpp helper header and the absolute root where they are staged.
# Consumers must additionally add a build dependency on xmipp4-test-assets.
add_library(xmipp4-test-asset-utils INTERFACE)
target_include_directories(
	xmipp4-test-asset-utils
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(
	xmipp4-test-asset-utils
	INTERFACE
		XMIPP4_TEST_ASSET_ROOT="${XMIPP4_TEST_ASSET_ROOT}"
)
