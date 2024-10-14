# honesty module definition
target_sources(honesty
	PUBLIC # For building the library and dependents
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			honesty.ixx
	PRIVATE # For building the target
		FILE_SET honesty_private
		TYPE CXX_MODULES
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

target_link_libraries(honesty
	PUBLIC
		synodic::benchmark
		synodic::platform
		synodic::profile
		synodic::log
)
