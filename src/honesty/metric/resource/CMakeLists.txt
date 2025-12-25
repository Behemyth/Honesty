# Platform-specific resource collection implementations
if(WIN32)
	target_sources(honesty PRIVATE windows.cpp)
	# Link psapi library for GetProcessMemoryInfo
	target_link_libraries(honesty PRIVATE psapi)
elseif(UNIX AND NOT APPLE)
	target_sources(honesty PRIVATE linux.cpp)
elseif(APPLE)
	# macOS stub - can be expanded later with Mach APIs
	target_sources(honesty PRIVATE linux.cpp)
endif()
