# Platform-specific implementation files (module implementation units)
if(WIN32)
	target_sources(honesty PRIVATE windows.cpp)
	# Link Windows multimedia library for timer functions
	target_link_libraries(honesty PRIVATE winmm)
elseif(UNIX AND NOT APPLE)
	target_sources(honesty PRIVATE linux.cpp)
elseif(APPLE)
	# macOS stub - can be expanded later
	target_sources(honesty PRIVATE linux.cpp)
endif()
