export module synodic.honesty.trace:tracer;

import std;

import :context;
import :types;
import zstring_view;

namespace honesty::trace
{
	/**
	 * @brief A tracer is a tag for Trace objects.
	 */
	export
	class Tracer
	{
	public:
		/**
		 * @brief The compile-time configuration for a tracer.
		 */
		struct Configuration
		{
			consteval Configuration(const bool enabled = false) :
				enabled(enabled)
			{
			}

			bool enabled;
		};


		consteval const Configuration& GetConfiguration() const
		{
			return configuration_;
		}

	private:
		template<group_enum T>
		friend class Provider;

		consteval explicit Tracer(const Configuration& configuration) :
			configuration_(configuration)
		{
		}

		Configuration configuration_;
	};
}
