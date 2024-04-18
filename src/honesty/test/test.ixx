
export module synodic.honesty.test:test;

import std;
import function_ref;
import :generator;
import :types;
import :registry;

namespace synodic::honesty::test
{
	class VoidTest;

	template<typename FuncType, typename RetType>
	concept SameReturn = requires(FuncType func) {
		{
			func()
		} -> std::same_as<RetType>;
	};

	template<typename T>
	class TinyTest final : public TestBase
	{
	public:
		TinyTest(std::string_view name, std::function_ref<void(const T&)> test, T value);

		void Run() const override;
		std::span<std::string_view> Tags() const override;

		std::string_view Name() const override;

	private:
		std::function_ref<void(const T&)> runner_;
		T value_;
		std::string_view name_;
	};

	class VoidTest final : public TestBase
	{
	public:
		VoidTest(std::string_view name, std::function_ref<void()> test);

		VoidTest& operator=(std::function_ref<void()> test);

		void Run() const override;
		std::span<std::string_view> Tags() const override;

		std::string_view Name() const override;

	private:
		std::function_ref<void()> runner_;
		std::string_view name_;
	};

	// TODO: Distinguish type overloads
	export template<typename Fn>
		requires std::invocable<Fn> && SameReturn<Fn, generator<TestBase>>
	generator<TestBase> Test(std::string_view name, Fn&& generator)
	{
		for (const TestBase& test: generator())
		{
			// TODO: inject changes
			co_yield Test(
				"TODO",
				[]()
				{
				});
		}
	}

	export template<typename Fn>
		requires std::invocable<Fn> && SameReturn<Fn, void>
	VoidTest Test(std::string_view name, Fn&& generator)
	{
		return VoidTest(name, std::move(generator));
	}

	export template<typename Fn, typename T>
		requires std::invocable<Fn, T> && SameReturn<Fn, void>
	TinyTest<T> Test(std::string_view name, Fn&& generator)
	{
		return TinyTest(name, std::move(generator));
	}

	/**
	 * @brief Strongly typed definition around string_view with construction
	 */
	template<std::size_t Size = 0>
	class [[nodiscard]] TestStub
	{
	public:
		consteval TestStub(std::string_view name) :
			name_(name)
		{
		}

		TestStub(const TestStub& other)				   = delete;
		TestStub(TestStub&& other) noexcept			   = delete;
		TestStub& operator=(const TestStub& other)	   = delete;
		TestStub& operator=(TestStub&& other) noexcept = delete;

		generator<TestBase> operator=(generator<TestBase>&& generator) const;

		template<typename Fn>
			requires std::invocable<Fn> && SameReturn<Fn, generator<TestBase>>
		generator<TestBase> operator=(Fn&& generator)
		{
			return Test(name_, std::forward<Fn>(generator));
		}

		template<typename Fn>
			requires std::invocable<Fn> && SameReturn<Fn, void>
		VoidTest operator=(Fn&& generator)
		{
			return Test(name_, std::forward<Fn>(generator));
		}

	private:
		template<std::size_t>
		friend class TestStub;

		std::string_view name_;
	};

	template<typename T>
	TinyTest<T>::TinyTest(std::string_view name, std::function_ref<void(const T&)> test, T value) :
		runner_(test),
		value_(value),
		name_(name)
	{
	}

	template<typename T>
	void TinyTest<T>::Run() const
	{
		runner_(value_);
	}

	template<typename T>
	std::span<std::string_view> TinyTest<T>::Tags() const
	{
		return {};
	}

	template<typename T>
	std::string_view TinyTest<T>::Name() const
	{
		return name_;
	}

	export template<std::size_t Size>
	generator<TestBase> TestStub<Size>::operator=(generator<TestBase>&& generator) const
	{
		return generator;
	}

	// Operators

	export template<std::invocable<int> Fn>
	[[nodiscard]] generator<TestBase> operator|(Fn&& test, const std::ranges::range auto& range)
	{
		for (const auto& value: range)
		{
			co_yield TinyTest<decltype(value)>("", test, value);
		}
	}

	// recursion base case
	template<typename Fn, typename T>
	generator<TestBase> ApplyNested(Fn&& test, T arg)
	{
		co_yield TinyTest<T>("", std::forward<Fn>(test), arg);
	}

	// recursive function
	template<typename Fn, typename T, typename... Ts>
	generator<TestBase> ApplyNested(Fn&& test, T arg, Ts... rest)
	{
		co_yield TinyTest<T>("", std::forward<Fn>(test), arg);
		co_yield ApplyNested(std::forward<Fn>(test), rest...);
	}

	export template<typename Fn, typename... Types>
		requires(std::invocable<Fn, Types> && ...)
	[[nodiscard]] generator<TestBase> operator|(Fn&& test, std::tuple<Types...>&& tuple)
	{
		auto applicator = [&]<typename... T>(T&&... args) -> generator<TestBase>
		{
			co_yield ApplyNested(std::forward<Fn>(test), args...);
		};

		co_yield std::apply(applicator, tuple);
	}

	Registry& GetRegistry();
	RunnerContext& GetContext();

	export bool RegisterRunner(Runner& runner);
	export bool RegisterReporter(Reporter& reporter);

	/**
	 * @brief Allows the static registration of tests in the global scope
	 */
	export template<size_t Size>
	class Suite final : SuiteData
	{
	public:
		consteval Suite(const char (&name)[Size], std::function_ref<generator<TestBase>()> generator);

		Suite(const Suite& other)	  = delete;
		Suite(Suite&& other) noexcept = delete;

		Suite& operator=(const Suite& other)	 = delete;
		Suite& operator=(Suite&& other) noexcept = delete;

		bool Register() &
		{
			SuiteData& data = *this;
			GetRegistry().AddSuite(data);

			return true;
		}

	private:
		std::array<char, Size> name_;
	};

	template<size_t Size>
	consteval Suite<Size>::Suite(const char (&name)[Size], std::function_ref<generator<TestBase>()> generator) :
		name_ {0},
		SuiteData(std::string_view(name_.data(), Size), generator)
	{
		std::copy_n(name, Size, name_.begin());
	}

	/**
	 * @brief Registers a suite of tests with the global registry.
	 * @param ...suites Suites to be registered. Each are expected to be an l-value reference.
	 * @return TODO
	 */
	export template<size_t... Sizes>
	bool Register(Suite<Sizes>&... suites)
	{
		(suites.Register(), ...);
		return true;
	}

	export template<std::size_t Size>
	class Tag
	{
	public:
		explicit consteval Tag(
			std::convertible_to<std::string_view> auto t,
			std::convertible_to<std::string_view> auto... tags) :
			tags_ {t, tags...}
		{
		}

		template<std::size_t RSize>
		consteval Tag<Size + RSize> operator/(Tag<RSize> tag) const;

		consteval TestStub<Size> operator/(const TestStub<>& test) const;

	private:
		template<std::size_t>
		friend class Tag;

		std::array<std::string_view, Size> tags_;
	};

	template<std::size_t Size>
	consteval TestStub<Size> Tag<Size>::operator/(const TestStub<>& test) const
	{
		// TODO
		return TestStub<Size>("TODO: Tag operator /");
	}

	template<std::size_t Size>
	template<std::size_t RSize>
	consteval Tag<Size + RSize> Tag<Size>::operator/(Tag<RSize> tag) const
	{
		Tag<Size + RSize> result(tags_[0]);

		// Concat
		std::size_t index = 0;

		for (auto& el: tag.tags_)
		{
			result.tags_[index] = std::move(el);
			++index;
		}
		for (auto& el: tags_)
		{
			result.tags_[index] = std::move(el);
			++index;
		}

		return result;
	}

	export Tag(std::string_view) -> Tag<1>;

	export template<typename... T>
	Tag(std::string_view, T...) -> Tag<1 + sizeof...(T)>;

	export constexpr Tag skip("skip");

	export namespace literals
	{
		[[nodiscard]] consteval auto operator""_test(const char* const name, const std::size_t size)
		{
			return TestStub(std::string_view(name, size));
		}

		[[nodiscard]] consteval auto operator""_tag(const char* const name, const std::size_t size)
		{
			return Tag(std::string_view(name, size));
		}
	}
}
