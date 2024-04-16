# Overwrite possibly existing ${_CTEST_FILE} with empty file
set(flush_tests_MODE WRITE)

# Flushes script to ${_CTEST_FILE}
macro(flush_script)
	file(${flush_tests_MODE} "${_CTEST_FILE}" "${script}")
	message(FATAL_ERROR ${_CTEST_FILE} ${script})
	set(flush_tests_MODE APPEND PARENT_SCOPE)

	set(script "")
endmacro()

# Flushes tests_buffer to tests
macro(flush_tests_buffer)
	list(APPEND tests "${tests_buffer}")
	set(tests_buffer "")
endmacro()

function(add_command NAME TEST_NAME)
	set(args "")

	foreach(arg ${ARGN})
		if(arg MATCHES "[^-./:a-zA-Z0-9_]")
			string(APPEND args " [==[${arg}]==]")
		else()
			string(APPEND args " ${arg}")
		endif()
	endforeach()

	string(APPEND script "${NAME}(${TEST_NAME} ${args})\n")
	string(LENGTH "${script}" script_len)

	if(${script_len} GREATER "50000")
		flush_script()
	endif()

	set(script "${script}" PARENT_SCOPE)
endfunction()

function(honesty_extract_tests)
	cmake_parse_arguments(
		""
		""
		"EXECUTABLE;CTEST_FILE;WORKING_DIR;DISCOVERY_TIMEOUT"
		""
		${ARGN}
	)

	# Set function state variables
	set(script)
	set(tests)
	set(tests_buffer)

	# Run test executable to get list of available tests
	if(NOT EXISTS "${_EXECUTABLE}")
		message(FATAL_ERROR
			"Specified test executable does not exist.\n"
			"  Path: '${_EXECUTABLE}'"
		)
	endif()

	set(HONESTY_ARGS "--list-tests")

	cmake_path(GET _EXECUTABLE PARENT_PATH EXECUTABLE_PARENT)

	message("Running '${_EXECUTABLE} ${HONESTY_ARGS}' in directory '${EXECUTABLE_PARENT}'")

	# Extract the test list from the Honesty target
	execute_process(
		COMMAND ${_EXECUTABLE} ${HONESTY_ARGS}
		WORKING_DIRECTORY ${EXECUTABLE_PARENT}
		TIMEOUT ${_DISCOVERY_TIMEOUT}
		OUTPUT_VARIABLE output
		RESULT_VARIABLE result
	)

	string(REPLACE "\n" "\n    " output "${output}")
	set(path "${_EXECUTABLE}")

	# Handle process errors
	if(NOT ${result} EQUAL 0)
		message(FATAL_ERROR
			"Error running test executable.\n"
			"  Path: '${path}'\n"
			"  Working directory: '${_WORKING_DIR}'\n"
			"  Result: ${result}\n"
			"  Output:\n"
			"    ${output}\n"
		)
	else()
		message(
			"Test executable result.\n"
			"  Path: '${path}'\n"
			"  Working directory: '${_WORKING_DIR}'\n"
			"  Result: ${result}\n"
			"  Output:\n"
			"    ${output}\n"
		)
	endif()

	flush_tests_buffer()

	# Write CTest script
	flush_script()
endfunction()
