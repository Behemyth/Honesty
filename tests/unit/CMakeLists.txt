target_sources(tests
PRIVATE
	test_expect.cpp
	test_generator.cpp
	test_suite.cpp
	test_test.cpp
	test_entry.cpp
	test_set.cpp
)

add_test(NAME tests COMMAND tests)
