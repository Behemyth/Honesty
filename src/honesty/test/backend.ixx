export module synodic.honesty.test.backend;

import std;

import generator;
import counter;

template<int I>
struct U;

constexpr std::counter<U<1>> Counter;

export namespace synodic::honesty
{
	class TestBase
	{
	public:
		virtual ~TestBase() = default;
		virtual void Run()	= 0;

	private:
	};

	struct suite_data
	{
		consteval suite_data(std::string_view name, std::generator<TestBase> (*generator)()) noexcept;

		std::string_view name_;
		std::generator<TestBase> (*generator_)();
	};

	consteval suite_data::suite_data(std::string_view name, std::generator<TestBase> (*generator)()) noexcept :
		name_(name),
		generator_(generator)
	{
	}

	template<std::size_t>
	class suite_builder
	{
	public:
		static consteval void Initialize(suite_data data);
		static consteval suite_data Get();

	private:
		consteval suite_builder(suite_data data) noexcept;

		static consteval suite_data SetData(suite_data data);

		suite_data data_;
	};

	template<std::size_t T>
	consteval suite_builder<T>::suite_builder(suite_data data) noexcept :
		data_(data)
	{
	}

	template<std::size_t T>
	consteval void suite_builder<T>::Initialize(suite_data data)
	{
		SetData(data);
	}

	template<std::size_t T>
	consteval suite_data suite_builder<T>::Get()
	{
		return SetData(suite_data("Uninitialized Suite", nullptr));
	}

	template<std::size_t T>
	consteval suite_data suite_builder<T>::SetData(suite_data data)
	{
		// Alternative to static constexpr variable
		auto builder = [&]() consteval
		{
			return suite_data(data);
		};
		return builder();

		// TODO: Replace with this come C++23
		// static constexpr suite_builder data(name, generator);
		// return data;
	}

	consteval void AddSuite(suite_data data)
	{
		constexpr int nextIndex = Counter.next<__COUNTER__>();

		suite_builder<nextIndex>::Initialize(data);
	}

	consteval auto GenerateSuites()
	{
		constexpr int count = Counter.current<__COUNTER__>();

		auto sequenceGenerator = []<std::size_t... Indices>(std::index_sequence<Indices...>) -> auto
		{
			return std::array<suite_data, sizeof...(Indices)>{suite_builder<Indices>::Get()...};
		};

		return sequenceGenerator(std::make_index_sequence<count>());
	}
}
