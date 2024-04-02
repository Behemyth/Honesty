target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	entry.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	entry.cpp
)

nanobind_add_module(honesty_ext
	python.cpp)

target_link_libraries(honesty_ext PRIVATE honesty)