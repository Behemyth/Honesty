function(honesty_discover_tests TARGET)
    cmake_parse_arguments(
        ""
        ""
        "WORKING_DIRECTORY;DISCOVERY_TIMEOUT"
        ""
        ${ARGN}
    )

	# Handle argument defaults
	if(NOT _WORKING_DIRECTORY)
		set(_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
	endif()
	if(NOT _DISCOVERY_TIMEOUT)
		set(_DISCOVERY_TIMEOUT 5)
	endif()

endfunction()