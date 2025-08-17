target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			metric.ixx
			context.ixx
			types.ixx
			timer.ixx
	PRIVATE
		FILE_SET metric_private
		TYPE CXX_MODULES
)
