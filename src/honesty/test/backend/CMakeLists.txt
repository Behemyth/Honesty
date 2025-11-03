target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			module.ixx
			storage.ixx
			reporter_registry.ixx
			runner_registry.ixx
			suite_context.ixx
			test_context.ixx
			requirements.ixx
			test.ixx
			tracer.ixx
			reporter.ixx
			runner.ixx
			suite.ixx
			fixture.ixx
			tag.ixx
	PRIVATE
		FILE_SET test_backend_private
		TYPE CXX_MODULES
)
