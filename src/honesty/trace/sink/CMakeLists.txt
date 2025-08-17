target_sources(honesty
	PUBLIC
		FILE_SET honesty_public
		TYPE CXX_MODULES
		FILES
			otel.ixx
			log.ixx
			scatter.ixx
	PRIVATE
		FILE_SET trace_private
		TYPE CXX_MODULES
)
