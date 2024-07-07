target_sources(honesty_test
	PUBLIC
	FILE_SET interfaces
	TYPE CXX_MODULES
	FILES
	execute.ixx
	list.ixx
	types.ixx
	api.ixx
)
