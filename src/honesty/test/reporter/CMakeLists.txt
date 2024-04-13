target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			console.ixx
)

target_sources(honesty
	INTERFACE
	export.cpp
)
