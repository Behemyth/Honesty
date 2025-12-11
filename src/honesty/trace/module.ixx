export module synodic.honesty.trace;

export import :trace;
export import :tracer;
export import :types;
export import :span;
export import :scope;
export import :provider;
export import :context;

export import :sink;
export import :sink.log;
export import :sink.scatter;
export import :sink.otel;

namespace honesty
{
	export using Span = trace::Span;
	export using NoopSpan = trace::NoopSpan;
}
