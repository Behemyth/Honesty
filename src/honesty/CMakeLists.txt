target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			honesty.ixx
	PRIVATE
		FILE_SET honesty_private
		TYPE CXX_MODULES
)

add_subdirectory(
	metric
)

add_subdirectory(
	platform
)

add_subdirectory(
	trace
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
