target_sources(
	rexlib-cpu-obj
	PRIVATE
		program_builder.cpp
)

add_subdirectory(builders)
add_subdirectory(hardware)
add_subdirectory(kernels)
add_subdirectory(loops)
add_subdirectory(plans)
