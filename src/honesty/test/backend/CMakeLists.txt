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
			"reporter_registry.ixx"
			"runner_registry.ixx"
			suite_context.ixx
			test_context.ixx
			requirements.ixx
			test.ixx
			
			"reporter.ixx"
			"runner.ixx"
			suite.ixx
			fixture.ixx
			tag.ixx
)
