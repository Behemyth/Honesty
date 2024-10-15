target_sources(honesty
	PUBLIC
		FILE_SET test_backend_public
		TYPE CXX_MODULES
	PRIVATE
		FILE_SET test_backend_private
		TYPE CXX_MODULES
		FILES
			backend.ixx
			storage.ixx
			reporter.ixx
			runner.ixx
)
