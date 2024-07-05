# Define file sets for HONESTY
target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
)

target_sources(honesty
	PRIVATE
	FILE_SET implementations
	TYPE CXX_MODULES
)

# Define the file sets for the library
target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			"benchmark.ixx"
			"profile.ixx"
			"test.ixx"
			"log.ixx"
			"utility.ixx"
)

add_subdirectory(
	benchmark
)

add_subdirectory(
	platform
)

add_subdirectory(
	profile
)

add_subdirectory(
	log
)

add_subdirectory(
	test
)

add_subdirectory(
	utility
)
