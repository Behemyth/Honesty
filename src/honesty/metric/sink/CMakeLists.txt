target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			otel.ixx
	PRIVATE
		FILE_SET metric_sink_private
		TYPE CXX_MODULES
)
