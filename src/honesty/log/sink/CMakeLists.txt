target_sources(honesty
	PUBLIC
		FILE_SET interfaces
		FILES
			sink.ixx
			console.ixx
			ringbuffer.ixx
			synchronized.ixx
			ostream.ixx
			callback.ixx
)

