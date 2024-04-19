
export module synodic.honesty.test:test;

import std;
import function_ref;
import :types;
import :registry;

namespace synodic::honesty::test
{
	class VoidTest;

	template<typename FuncType, typename RetType>
	concept same_return = requires(FuncType func) {
		{
			func()
		} -> std::same_as<RetType>;
	};

	// TODO: Distinguish type overloads
	export template<typename Fn>
		requires std::invocable<Fn>
	void Test(std::string_view name, Fn&& generator)
	{
		
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

		void operator=() const;

		template<typename Fn>
			requires std::invocable<Fn>
		void operator=(Fn&& generator)
		{
			return Test(name_, std::forward<Fn>(generator));
		}

		template<typename Fn>
			requires std::invocable<Fn>
		VoidTest operator=(Fn&& generator)
		{
			return Test(name_, std::forward<Fn>(generator));
		}

	private:
		template<std::size_t>
		friend class TestStub;

		std::string_view name_;
	};

	// Operators

	export template<std::invocable<int> Fn>
	void operator|(Fn&& test, const std::ranges::range auto& range)
	{
		for (const auto& value: range)
		{
			TinyTest<decltype(value)>("", test, value);
		}
	}

	// recursion base case
	template<typename Fn, typename T>
	void ApplyNested(Fn&& test, T arg)
	{
		TinyTest<T>("", std::forward<Fn>(test), arg);
	}

	// recursive function
	template<typename Fn, typename T, typename... Ts>
	void ApplyNested(Fn&& test, T arg, Ts... rest)
	{
		TinyTest<T>("", std::forward<Fn>(test), arg);
		ApplyNested(std::forward<Fn>(test), rest...);
	}

	export template<typename Fn, typename... Types>
		requires(std::invocable<Fn, Types> && ...)
	void operator|(Fn&& test, std::tuple<Types...>&& tuple)
	{
		auto applicator = [&]<typename... T>(T&&... args) -> void
		{
			ApplyNested(std::forward<Fn>(test), args...);
		};

		std::apply(applicator, tuple);
	}

	Registry& GetRegistry();
	RunnerContext& GetContext();

	export bool RegisterRunner(Runner& runner);
	export bool RegisterReporter(Reporter& reporter);

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
