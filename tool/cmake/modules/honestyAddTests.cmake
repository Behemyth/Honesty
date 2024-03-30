
function(honesty_extract_tests)

    cmake_parse_arguments(
        ""
        ""
        "TEST_EXECUTABLE;CTEST_FILE;TEST_WORKING_DIR;TEST_DISCOVERY_TIMEOUT"
        ""
        ${ARGN}
    )

	# Run test executable to get list of available tests
	if(NOT EXISTS "${_TEST_EXECUTABLE}")
		message(FATAL_ERROR
			"Specified test executable does not exist.\n"
			"  Path: '${_TEST_EXECUTABLE}'"
		)
	endif()

	# Extract the test list from the Honesty target
	execute_process(
		COMMAND "${_TEST_EXECUTABLE}" --list-tests
		WORKING_DIRECTORY "${_TEST_WORKING_DIR}"
		TIMEOUT ${_TEST_DISCOVERY_TIMEOUT}
		OUTPUT_VARIABLE output
		RESULT_VARIABLE result
	)

	# Handle process errors
	if(NOT ${result} EQUAL 0)
		string(REPLACE "\n" "\n    " output "${output}")
		set(path "${_TEST_EXECUTABLE}")

		message(FATAL_ERROR
		  "Error running test executable.\n"
		  "  Path: '${path}'\n"
		  "  Working directory: '${_TEST_WORKING_DIR}'\n"
		  "  Result: ${result}\n"
		  "  Output:\n"
		  "    ${output}\n"
		)
	endif()

endfunction()