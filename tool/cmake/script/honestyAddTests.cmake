# Overwrite possibly existing ${_CTEST_FILE} with empty file
set(flush_tests_MODE WRITE)

function(honesty_extract_tests)
	cmake_parse_arguments(
		""
		""
		"EXECUTABLE;CTEST_FILE;WORKING_DIR;DISCOVERY_TIMEOUT"
		""
		${ARGN}
	)

	# Set function state variables
	set(ctest_output)

	# Run test executable to get list of available tests
	if(NOT EXISTS ${_EXECUTABLE})
		message(FATAL_ERROR
			"Specified test executable does not exist.\n"
			"  Path: '${_EXECUTABLE}'"
		)
	endif()

	set(HONESTY_ARGS list --json --file honesty_test.json)

	cmake_path(GET _EXECUTABLE PARENT_PATH EXECUTABLE_PARENT)

	message("Running command:\n	'${HONESTY_ARGS}'")

	# Extract the test list from the Honesty target
	execute_process(
		COMMAND ${_EXECUTABLE} ${HONESTY_ARGS}
		WORKING_DIRECTORY ${_WORKING_DIRECTORY}
		TIMEOUT ${_DISCOVERY_TIMEOUT}
		OUTPUT_VARIABLE output
		ERROR_VARIABLE output
		RESULT_VARIABLE result
	)

	# Indent the output for better logging readability
	string(REPLACE "\n" "\n    " output "${output}")

	# Log the results of the process
	if(NOT ${result} EQUAL 0)
		message(FATAL_ERROR
			"Error running test executable.\n"
			"  Path: '${_EXECUTABLE}'\n"
			"  Working directory: '${_WORKING_DIR}'\n"
			"  Result: ${result}\n"
			"  Output:\n"
			"    ${output}\n"
		)
	else()
		message(
			"Test executable result.\n"
			"  Path: '${_EXECUTABLE}'\n"
			"  Working directory: '${_WORKING_DIR}'\n"
			"  Output:\n"
			"    ${output}\n"
		)
	endif()

	cmake_path(SET full_path "${_WORKING_DIR}/honesty_test.json")

	# Parse the JSON output into a CMake string
	file(READ ${full_path} json_buffer)

	# Get the number of tests
	string(JSON test_count LENGTH ${json_buffer} "tests")

	# Create a list of indices to iterate over
	set(indices "")

	foreach(index RANGE ${test_count})
		list(APPEND indices ${index})
	endforeach()

	# Parse the top-level json string and get the tests array
	string(JSON tests_buffer GET ${json_buffer} "tests")

	# Process each test name
	foreach(test_name ${tests_buffer})
		set(HONESTY_ARGS --filter ${test_name})

		string(APPEND ctest_output
			"add_test(NAME ${test_name}" "\n"
			"    COMMAND ${_EXECUTABLE} ${HONESTY_ARGS}" "\n"
			")" "\n"
		)
	endforeach()

	if(NOT ctest_output)
		message(WARNING "No tests have been discovered.")
	endif()

	# Write the generated ctest file
	file(WRITE ${_CTEST_FILE} ${ctest_output})
endfunction()
