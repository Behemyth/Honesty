
target_sources(honesty
	PUBLIC
		FILE_SET test_types_public
		TYPE CXX_MODULES
		FILES
			requirements.ixx
			test.ixx
			types.ixx
			reporter.ixx
			runner.ixx
			suite.ixx
			fixture.ixx
			tag.ixx
	PRIVATE
		FILE_SET test_types_private
		TYPE CXX_MODULES
)
