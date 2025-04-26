target_sources(honesty
	PUBLIC
		FILE_SET trace_public
		TYPE CXX_MODULES
		FILES
			context.ixx
			trace.ixx
			module.ixx
			provider.ixx
			tracer.ixx
			types.ixx
			scope.ixx
			span.ixx
	PRIVATE
		FILE_SET trace_private
		TYPE CXX_MODULES
)

add_subdirectory(sink)
