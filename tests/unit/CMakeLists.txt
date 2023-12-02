target_sources(tests
PRIVATE
	test_expect.cpp
	test_generator.cpp
	test_suite.cpp
	test_test.cpp
	test_entry.cpp
	test_logger.cpp
	test_runner.cpp
	test_reporter.cpp
	test_tag.cpp
	test_exception.cpp
)

add_test(NAME tests COMMAND tests)
