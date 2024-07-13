target_sources(honesty
	PRIVATE
	FILE_SET private_interfaces
	TYPE CXX_MODULES
	FILES
	backend.ixx
	data.ixx
	storage.ixx
)
