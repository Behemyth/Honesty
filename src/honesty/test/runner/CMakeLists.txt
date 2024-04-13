target_sources(honesty
	PUBLIC
	FILE_SET interfaces
	FILES
	local.ixx
)

target_sources(honesty
	INTERFACE
	export.cpp
)
