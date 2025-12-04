add_subdirectory(dummy_plugin)
add_subdirectory(faulty_plugin1)
add_subdirectory(faulty_plugin2)

set(PLUGINS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plugins/)
add_custom_target(
	xmipp4-core-integration-test-assets
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lorem_ipsum.txt ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dummy-plugin> ${PLUGINS_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:faulty-plugin1> ${PLUGINS_DIRECTORY}
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:faulty-plugin2> ${PLUGINS_DIRECTORY}
)
