export module synodic.honesty.test:assert;

import std;
import :test;

namespace synodic::honesty::test
{
	/**
	 * @brief Asserts that the expression is true. Fatal on failure.
	 * @param expression The expression to evaluate.
	 * @param location The source location. Let this parameter be defaulted.
	 */
	export void assert(bool expression, const std::source_location& location = std::source_location::current());

	/**
	 * @brief Asserts that the expression is true. Nonfatal on failure.
	 * @param expression The expression to evaluate.
	 * @param location The source location. Let this parameter be defaulted.
	 */
	export bool expect(bool expression, const std::source_location& location = std::source_location::current());

	export template<std::convertible_to<bool> T>
	void assert(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return assert(static_cast<bool>(expression), location);
	}

	export template<std::convertible_to<bool> T>
	bool expect(const T& expression, const std::source_location& location = std::source_location::current())
	{
		return expect(static_cast<bool>(expression), location);
	}

	export template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
	constexpr void assert_throw(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}
		catch (const Exception&)
		{
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}
	}

	export template<std::invocable Fn>
	constexpr void
		assert_not_throw(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}
	}

	export template<std::derived_from<std::exception> Exception = std::exception, std::invocable Fn>
	constexpr void expect_throw(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}
		catch (const Exception&)
		{
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}
	}

	export template<std::invocable Fn>
	constexpr void
		expect_not_throw(Fn&& function, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::invoke(std::forward<Fn>(function));
			event::AssertionPass passed;
			passed.location = location;

			GetContext().broadcaster.Signal(passed);
		}
		catch (...)
		{
			event::AssertionFail failed;
			failed.location = location;

			GetContext().broadcaster.Signal(failed);
		}
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr bool
		assert_equals(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return assert(a == b, location);
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr bool assert_not_equals(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return assert(a != b, location);
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr bool
		expect_equals(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return expect(a == b, location);
	}

	export template<class T, class U>
		requires std::equality_comparable_with<T, U>
	constexpr bool expect_not_equals(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return expect(a != b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool
		assert_greater(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return assert(a > b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool
		assert_less(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return assert(a < b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool assert_greater_equal(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return assert(a >= b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool assert_less_equal(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return assert(a <= b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool
		expect_greater(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return expect(a > b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool
		expect_less(const T& a, const U& b, const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return expect(a < b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool expect_greater_equal(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return expect(a >= b, location);
	}

	export template<class T, class U>
		requires std::totally_ordered_with<T, U>
	constexpr bool expect_less_equal(
		const T& a,
		const U& b,
		const std::source_location& location = std::source_location::current())
	{
		// TODO: Pass message context
		return expect(a <= b, location);
	}
}
