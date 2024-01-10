target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		TYPE CXX_MODULES
		FILES
			"test.ixx"
			"function_ref.ixx"
			"counter.ixx"
	PRIVATE
		"test.cpp"
)

target_sources(honesty_main
	PUBLIC
		"main.cpp"
)

add_subdirectory(
	entry
)

add_subdirectory(
	backend
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)

add_subdirectory(
	logger
)