# Platform-specific implementation files (module implementation units)
if(WIN32)
	target_sources(honesty PRIVATE windows.cpp)
	# Link Windows multimedia library for timer functions
	target_link_libraries(honesty PRIVATE winmm)
else()
	message(FATAL_ERROR "Platform tuning not yet implemented for this platform")
endif()
