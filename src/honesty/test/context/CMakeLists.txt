target_sources(honesty
	PRIVATE
	FILE_SET private_interfaces
	TYPE CXX_MODULES
	FILES
	context.ixx
	data.ixx
	types.ixx
)
