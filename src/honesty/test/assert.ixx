export module synodic.honesty.test:assert;

import std;
import :test;
import :runner;

namespace synodic::honesty::test
{

	/**
	 * @brief Internal function to signal an assertion passed.
	 * @param passed 
	 */
	void Signal(const event::AssertionPass& passed);

	/**
	 * @brief Internal function to signal an assertion failure.
	 * @param failed 
	 */
	void Signal(const event::AssertionFail& failed);

	/**
	 * @brief Asserts that the expression is true. Fatal on failure.
	 * @param expression The expression to evaluate.
	 * @param location The source location. Let this parameter be defaulted.
	 */
	export void Assert(bool expression, const std::source_location& location = std::source_location::current());

	/**
	 * @brief Asserts that the expression is true. Nonfatal on failure.
	 * @param expression The expression to evaluate.
	 * @param location The source location. Let this parameter be defaulted.
	 */
	export void Expect(bool expression, const std::source_location& location = std::source_location::current());

	export template<std::convertible_to<bool> T>
	void Assert(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return Assert(static_cast<bool>(expression), location);
	}

	export template<std::convertible_to<bool> T>
	void Expect(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return Expect(static_cast<bool>(expression), location);
	}

	export template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
	constexpr void AssertThrow(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
		catch (const Exception&)
		{
			event::AssertionPass passed;
			passed.location = location;

			Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
	}

	export template<std::invocable Fn>
	constexpr void
		AssertNotThrow(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionPass passed;
			passed.location = location;

			Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
	}

	export template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
	constexpr void ExpectThrow(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
		catch (const Exception&)
		{
			event::AssertionPass passed;
			passed.location = location;

			Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
	}

	export template<std::invocable Fn>
	constexpr void
		ExpectNotThrow(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionPass passed;
			passed.location = location;

			Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			Signal(failed);
		}
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr void
		AssertEquals(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Assert(a == b, location);
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr void AssertNotEquals(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Assert(a != b, location);
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr void
		ExpectEquals(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Expect(a == b, location);
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr void ExpectNotEquals(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Expect(a != b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void
		AssertGreater(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Assert(a > b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void
		AssertLess(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Assert(a < b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void AssertGreaterEqual(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Assert(a >= b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void AssertLessEqual(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Assert(a <= b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void
		ExpectGreater(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Expect(a > b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void
		ExpectLess(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Expect(a < b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void ExpectGreaterEqual(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Expect(a >= b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr void ExpectLessEqual(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		Expect(a <= b, location);
	}
}
