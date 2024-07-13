export module synodic.honesty.test.types:assert;

import std;

import :reporter;

import synodic.honesty.log;
import synodic.honesty.test.types;

namespace synodic::honesty::test
{
	export struct RequirementsContext
	{
		RequirementsContext() :
			success(true)
		{
		}

		bool success;
	};

	export class Requirements
	{
	public:
		Requirements(
			const std::span<std::unique_ptr<Reporter>> reporters,
			const std::string_view testName,
			const log::Logger& logger) :
			reporters_(reporters),
			testName_(testName),
			logger_(logger)
		{
		}

		/**
		 * @brief Asserts that the expression is true. Fatal on failure.
		 * @param expression The expression to evaluate.
		 * @param descriptionCallback If provided, evaluated and logged if the requirement fails as a test description
		 * @param location The source location. Let this parameter be defaulted.
		 */
		void Assert(
			const bool expression,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (expression)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		/**
		 * @brief Asserts that the expression is true. Fatal on failure.
		 * @param expression The expression to evaluate.
		 * @param description If provided, logged if the requirement fails as a test description
		 * @param location The source location. Let this parameter be defaulted.
		 */
		inline void Assert(
			const bool expression,
			const std::string_view description,
			const std::source_location& location = std::source_location::current()) const
		{
			Assert(
				expression,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		/**
		 * @brief Asserts that the expression is true. Nonfatal on failure.
		 * @param expression The expression to evaluate.
		 * @param descriptionCallback If provided, evaluated and logged if the requirement fails as a test description
		 * @param location The source location. Let this parameter be defaulted.
		 */
		void Expect(
			const bool expression,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (expression)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, descriptionCallback());

				Signal(failed);
			}
		}

		/**
		 * @brief Asserts that the expression is true. Nonfatal on failure.
		 * @param expression The expression to evaluate.
		 * @param description If provided, logged if the requirement fails as a test description
		 * @param location The source location. Let this parameter be defaulted.
		 */
		inline void Expect(
			const bool expression,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			Expect(
				expression,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<std::convertible_to<bool> T>
		inline void Assert(
			const T& expression,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			return Assert(static_cast<bool>(expression), descriptionCallback, location);
		}

		template<std::convertible_to<bool> T>
		inline void Assert(
			const T& expression,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			return Assert(static_cast<bool>(expression), description, location);
		}

		template<std::convertible_to<bool> T>
		inline void Expect(
			const T& expression,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			return Expect(static_cast<bool>(expression), descriptionCallback, location);
		}

		template<std::convertible_to<bool> T>
		inline void Expect(
			const T& expression,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			return Expect(static_cast<bool>(expression), description, location);
		}

		template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
		void AssertThrow(
			Fn&& function,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionFail failed(location, descriptionCallback(), true);

				Signal(failed);
			}
			catch (const Exception&)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location, descriptionCallback(), true);

				Signal(failed);
			}
		}

		template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
		inline void AssertThrow(
			Fn&& function,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertThrow<Exception>(
				std::forward<Fn>(function),
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<std::invocable Fn>
		void AssertNotThrow(
			Fn&& function,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location, descriptionCallback(), true);

				Signal(failed);
			}
		}

		template<std::invocable Fn>
		inline void AssertNotThrow(
			Fn&& function,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertNotThrow(
				std::forward<Fn>(function),
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
		void ExpectThrow(
			Fn&& function,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionFail failed(location, descriptionCallback());

				Signal(failed);
			}
			catch (const Exception&)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location, descriptionCallback());

				Signal(failed);
			}
		}

		template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
		inline void ExpectThrow(
			Fn&& function,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectThrow<Exception>(
				std::forward<Fn>(function),
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<std::invocable Fn>
		void ExpectNotThrow(
			Fn&& function,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location, descriptionCallback());

				Signal(failed);
			}
		}

		template<std::invocable Fn>
		inline void ExpectNotThrow(
			Fn&& function,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectNotThrow(
				std::forward<Fn>(function),
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		void AssertEquals(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a == b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::EqualityFail
					failed(location, false, std::format("{}", a), std::format("{}", b), descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		inline void AssertEquals(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertEquals(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		void AssertNotEquals(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a != b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::EqualityFail
					failed(location, true, std::format("{}", a), std::format("{}", b), descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		inline void AssertNotEquals(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertNotEquals(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		/**
		 * @brief A non-fatal assertion that both inputs are equal. If this function is being used it is expected that
		 *		the inputs are formattable to express contextual information
		 */
		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		void ExpectEquals(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a == b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::EqualityFail
					failed(location, false, std::format("{}", a), std::format("{}", b), descriptionCallback());

				Signal(failed);
			}
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		inline void ExpectEquals(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectEquals(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		void ExpectNotEquals(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a != b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::EqualityFail
					failed(location, true, std::format("{}", a), std::format("{}", b), descriptionCallback());

				Signal(failed);
			}
		}

		template<typename T, typename U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		inline void ExpectNotEquals(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectNotEquals(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void AssertGreater(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_gt(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void AssertGreater(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertGreater(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void AssertLess(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_lt(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void AssertLess(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertLess(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void AssertGreaterEqual(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_gteq(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void AssertGreaterEqual(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertGreaterEqual(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void AssertLessEqual(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_lteq(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback(), true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void AssertLessEqual(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			AssertLessEqual(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void ExpectGreater(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_gt(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback());

				Signal(failed);
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void ExpectGreater(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectGreater(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void ExpectLess(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_lt(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback());

				Signal(failed);
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void ExpectLess(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectLess(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void ExpectGreaterEqual(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_gteq(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback());

				Signal(failed);
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void ExpectGreaterEqual(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectGreaterEqual(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		void ExpectLessEqual(
			const T& a,
			const U& b,
			const std::function_ref<std::string()> descriptionCallback,
			const std::source_location& location = std::source_location::current()) const
		{
			if (const std::strong_ordering order = a <=> b; std::is_lteq(order))
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::ComparisonFail
					failed(location, order, std::format("{}", a), std::format("{}", b), descriptionCallback());

				Signal(failed);
			}
		}

		template<typename T, typename U>
			requires std::three_way_comparable_with<T, U, std::strong_ordering> && std::formattable<T, char> &&
					 std::formattable<U, char>
		inline void ExpectLessEqual(
			const T& a,
			const U& b,
			const std::string_view description	 = "",
			const std::source_location& location = std::source_location::current()) const
		{
			ExpectLessEqual(
				a,
				b,
				[&description]() -> std::string
				{
					return std::string(description);
				},
				location);
		}

		std::string_view TestName() const
		{
			return testName_;
		}

	protected:
		mutable RequirementsContext context_;

	private:
		/**
		 * @brief Internal function to signal an assertion passed.
		 * @param passed
		 */
		void Signal(const event::AssertionPass& passed) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(passed);
			}
		}

		/**
		 * @brief Internal function to signal an assertion failure.
		 * @param failed
		 */
		void Signal(const event::AssertionFail& failed) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(failed);
			}

			context_.success = false;
		}

		/**
		 * @brief Internal function to signal an equality failure.
		 * @param failed
		 */
		void Signal(const event::EqualityFail& failed) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(failed);
			}

			context_.success = false;
		}

		/**
		 * @brief Internal function to signal an ordering failure.
		 * @param failed
		 */
		void Signal(const event::ComparisonFail& failed) const
		{
			for (const std::unique_ptr<Reporter>& reporter: reporters_)
			{
				reporter->Signal(failed);
			}

			context_.success = false;
		}

		std::span<std::unique_ptr<Reporter>> reporters_;
		std::string_view testName_;
		std::reference_wrapper<const log::Logger> logger_;
	};

	export class RequirementsBackend : public Requirements
	{
	public:
		RequirementsBackend(
			std::span<std::unique_ptr<Reporter>> reporters,
			const std::string_view testName,
			const log::Logger& logger) :
			Requirements(reporters, testName, logger)
		{
		}

		const RequirementsContext& Context() const
		{
			return context_;
		}
	};

	export using Req = Requirements;

}
