target_sources(honesty
	PUBLIC
		FILE_SET test_api_public
		TYPE CXX_MODULES
		FILES
			execute.ixx
			list.ixx
			types.ixx
			api.ixx
	PRIVATE
		FILE_SET test_api_private
		TYPE CXX_MODULES
)

add_subdirectory(
	runner
)

add_subdirectory(
	reporter
)

target_sources(honesty_bindings
PUBLIC
	bindings.cpp
)
