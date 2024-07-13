target_sources(honesty
	PRIVATE
	FILE_SET private_interfaces
	TYPE CXX_MODULES
	FILES
	assert.ixx
	backend.ixx
	data.ixx
	fixture.ixx
	reporter.ixx
	runner.ixx
	storage.ixx
)
