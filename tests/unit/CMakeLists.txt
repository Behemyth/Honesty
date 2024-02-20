target_sources(tests
PRIVATE
	test_expression.cpp
	test_suite.cpp
	test_test.cpp
	test_logger.cpp
	test_runner.cpp
	test_reporter.cpp
	test_tag.cpp
	test_terminal.cpp
	test_exception.cpp
)

add_test(NAME tests COMMAND tests)
