target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	local.ixx
	PRIVATE
	FILE_SET implementations
	FILES
	local.cpp
)

target_sources(honesty
	INTERFACE
	export.cpp
)
