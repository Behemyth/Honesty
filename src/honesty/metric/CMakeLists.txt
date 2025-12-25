target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			metric.ixx
			comparison.ixx
			context.ixx
			escape.ixx
			history.ixx
			types.ixx
			timer.ixx
			results.ixx
			platform.ixx
	PRIVATE
		FILE_SET metric_private
		TYPE CXX_MODULES
)

add_subdirectory(platform)
add_subdirectory(sink)
