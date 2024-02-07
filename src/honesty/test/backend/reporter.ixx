export module synodic.honesty.test.backend:reporter;

import :logger;
import std;
import function_ref;

export namespace synodic::honesty
{
	struct TestBeginEvent
	{
		std::string_view name;
	};

	struct TestEndEvent
	{
		std::string_view name;
	};

	struct TestSkipEvent
	{
		std::string_view name;
	};

	struct TestRunEvent
	{
		std::string_view name;
	};

	struct TestFailEvent
	{
		std::string_view name;
	};

	struct TestPassEvent
	{
		std::string_view name;
	};

	struct AssertionFailEvent
	{
		std::string_view name;
	};

	struct AssertionPassEvent
	{
		std::string_view name;
	};

	struct AssertionSkipEvent
	{
		std::string_view name;
	};

	struct SummaryEvent
	{
		std::string_view name;
	};

	template<typename Event>
	class EventBroadcaster
	{
	public:
		void connect(std::move_only_function<void(const Event&)> connection)
		{
			connections_.push_back(std::move(connection));
		}

		void signal(const Event& event)
		{
			for (const auto& connection: connections_)
			{
				connection(event);
			}
		}

	private:
		std::vector<std::move_only_function<void(const Event&)>> connections_;
	};

	template<typename... Events>
	class EventSystem : EventBroadcaster<Events>...
	{
	public:
		template<typename Event>
			requires std::disjunction_v<Event, Events...>
		void connect(std::move_only_function<void(const Event&)> connection)
		{
			EventBroadcaster<Event>& backend = *this;
			backend.connect(std::move(connection));
		}

		template<typename Event>
			requires std::disjunction_v<Event, Events...>
		void signal(const Event& event)
		{
			EventBroadcaster<Event>& backend = *this;
			backend.signal(event);
		}
	};

	// TODO: Wait until deducing this is supported with modules. Using CRTP dispatch for events based on the type of the event

	class

	class Reporter :
		public EventSystem<
			TestBeginEvent,
			TestEndEvent,
			TestSkipEvent,
			TestRunEvent,
			TestFailEvent,
			TestPassEvent,
			AssertionFailEvent,
			AssertionPassEvent,
			AssertionSkipEvent,
			SummaryEvent>
	{
	public:
		Reporter()			= default;
		virtual ~Reporter() = default;

	private:
	};
}
