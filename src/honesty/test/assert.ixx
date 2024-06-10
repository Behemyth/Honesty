export module synodic.honesty.test:assert;

import std;
import :reporter;

namespace synodic::honesty::test
{
	export class Requirements
	{
	public:
		explicit Requirements(const std::span<Reporter*> reporters) :
			reporters_(reporters) {};

		/**
		 * @brief Asserts that the expression is true. Fatal on failure.
		 * @param expression The expression to evaluate.
		 * @param location The source location. Let this parameter be defaulted.
		 */
		void Assert(const bool expression, const std::source_location& location = std::source_location::current()) const
		{
			if (expression)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true);

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		/**
		 * @brief Asserts that the expression is true. Nonfatal on failure.
		 * @param expression The expression to evaluate.
		 * @param location The source location. Let this parameter be defaulted.
		 */
		void Expect(const bool expression, const std::source_location& location = std::source_location::current()) const
		{
			if (expression)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location);

				Signal(failed);
			}
		}

		template<std::convertible_to<bool> T>
		inline void
			Assert(const T& expression, const std::source_location& location = std::source_location::current()) const
		{
			return Assert(static_cast<bool>(expression), location);
		}

		template<std::convertible_to<bool> T>
		inline void
			Expect(const T& expression, const std::source_location& location = std::source_location::current()) const
		{
			return Expect(static_cast<bool>(expression), location);
		}

		template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
		constexpr void
			AssertThrow(Fn&& function, const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionFail failed(location);

				Signal(failed);
			}
			catch (const Exception&)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location);

				Signal(failed);
			}
		}

		template<std::invocable Fn>
		constexpr void
			AssertNotThrow(Fn&& function, const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location);

				Signal(failed);
			}
		}

		template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
		constexpr void
			ExpectThrow(Fn&& function, const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				const event::AssertionFail failed(location);

				Signal(failed);
			}
			catch (const Exception&)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				const event::AssertionFail failed(location);

				Signal(failed);
			}
		}

		template<std::invocable Fn>
		constexpr void
			ExpectNotThrow(Fn&& function, const std::source_location& location = std::source_location::current()) const
		{
			try
			{
				std::invoke(std::forward<Fn>(function));
				event::AssertionPass passed(location);

				Signal(passed);
			}
			catch (...)
			{
				event::AssertionFail failed(location);

				Signal(failed);
			}
		}

		template<class T, class U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void
			AssertEquals(const T& a, const U& b, const std::source_location& location = std::source_location::current())
				const
		{
			if (a == b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true, std::format("{} did not equal {}", a, b));

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<class T, class U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void AssertNotEquals(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a != b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true, std::format("{} was equal to {}", a, b));

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		/**
		 * @brief A non-fatal assertion that both inputs are equal. If this function is being used it is expected that
		 *		the inputs are formattable to express contextual information
		 */
		template<class T, class U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void
			ExpectEquals(const T& a, const U& b, const std::source_location& location = std::source_location::current())
				const
		{
			if (a == b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, false, std::format("{} did not equal {}", a, b));

				Signal(failed);
			}
		}

		template<class T, class U>
			requires std::equality_comparable_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void ExpectNotEquals(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a != b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, false, std::format("{} was equal to {}", a, b));

				Signal(failed);
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void AssertGreater(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a > b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true, std::format("{} was not greater than {}", a, b));

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void
			AssertLess(const T& a, const U& b, const std::source_location& location = std::source_location::current())
				const
		{
			if (a < b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true, std::format("{} was not less than {}", a, b));

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void AssertGreaterEqual(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a >= b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true, std::format("{} was less than {}", a, b));

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void AssertLessEqual(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a <= b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, true, std::format("{} was greater than {}", a, b));

				Signal(failed);
				throw AssertException("Assertion failed");
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void ExpectGreater(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a > b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, false, std::format("{} was not greater than {}", a, b));

				Signal(failed);
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void
			ExpectLess(const T& a, const U& b, const std::source_location& location = std::source_location::current())
				const
		{
			if (a < b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, false, std::format("{} was not less than {}", a, b));

				Signal(failed);
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void ExpectGreaterEqual(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a >= b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, false, std::format("{} was less than {}", a, b));

				Signal(failed);
			}
		}

		template<class T, class U>
			requires std::totally_ordered_with<T, U> && std::formattable<T, char> && std::formattable<U, char>
		constexpr void ExpectLessEqual(
			const T& a,
			const U& b,
			const std::source_location& location = std::source_location::current()) const
		{
			if (a <= b)
			{
				const event::AssertionPass passed(location);

				Signal(passed);
			}
			else
			{
				const event::AssertionFail failed(location, false, std::format("{} was greater than {}", a, b));

				Signal(failed);
			}
		}

	private:
		/**
		 * @brief Internal function to signal an assertion passed.
		 * @param passed
		 */
		void Signal(const event::AssertionPass& passed) const
		{
			for (Reporter* reporter: reporters_)
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
			for (Reporter* reporter: reporters_)
			{
				reporter->Signal(failed);
			}
		}

		std::span<Reporter*> reporters_;
	};
}
