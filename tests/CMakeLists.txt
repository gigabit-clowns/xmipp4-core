include(rexlib_add_catch2)
include(rexlib_add_trompeloeil)

# Declared here rather than beside the other REXLIB_USE_SYSTEM_* options,
# because these two dependencies exist only while the tests are built. They
# still take their default from the same global option.
option(REXLIB_USE_SYSTEM_CATCH2 "Use find_package for Catch2" ${REXLIB_USE_SYSTEM_DEPENDENCIES})
option(REXLIB_USE_SYSTEM_TROMPELOEIL "Use find_package for trompeloeil" ${REXLIB_USE_SYSTEM_DEPENDENCIES})

rexlib_add_catch2(VERSION 3.16.0 MINIMUM 3.0.1)
rexlib_add_trompeloeil(VERSION 49 MINIMUM 42)

include(Catch)

function(rexlib_register_tests target)
	if(REXLIB_REGISTER_TESTS_PER_BINARY)
		add_test(NAME ${target} COMMAND ${target})
		set_tests_properties(${target} PROPERTIES TIMEOUT 3600)
	else()
		catch_discover_tests(${target})
	endif()
endfunction()

add_subdirectory(assets)
add_subdirectory(unitary)
add_subdirectory(integration)
