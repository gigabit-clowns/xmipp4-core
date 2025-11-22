add_subdirectory(dummy_plugin)
add_subdirectory(faulty_plugin1)
add_subdirectory(faulty_plugin2)

set(TARGET_NAME "${PROJECT_NAME}_integration_test_assets")
set(PLUGINS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plugins/)
add_custom_target(
		${TARGET_NAME}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lorem_ipsum.txt ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGINS_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dummy_plugin> ${PLUGINS_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:faulty_plugin1> ${PLUGINS_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:faulty_plugin2> ${PLUGINS_DIRECTORY}
)
