export module synodic.honesty.trace:provider;

import std;

import :tracer;
import :types;
import :span;
import zstring_view;
import inplace_vector;

namespace honesty::trace
{
	/**
	* @brief A simple thread-local ring buffer for storing spans. We don't have uses for a generic ring
	*	buffer in this project. Not thread-safe
	* @tparam Capacity The capacity of the ring buffer. Must be a power of 2.
	*/
	template<std::size_t Capacity>
		requires (Capacity > 0 && (Capacity & Capacity - 1) == 0)
	class SpanRingBuffer
	{
	public:
		SpanRingBuffer() = default;

		// Adds an element to the buffer. Overwrites the oldest element if the buffer is full.
		void Push(const Span& span)
		{
			buffer_[head_] = span;
			head_          = head_ + 1 & Capacity - 1; // Wrap index

			if (size_ < Capacity)
			{
				++size_;
			}
			else
			{
				tail_ = tail_ + 1 & Capacity - 1; // Advance tail if buffer is full
			}
		}

		// Removes and returns the oldest element from the buffer. Assumes the buffer is not empty.
		Span Pop()
		{
			Span span = buffer_[tail_];
			tail_     = tail_ + 1 & Capacity - 1; // Wrap index

			--size_;

			return span;
		}

		// Checks if the buffer is empty
		bool Empty() const
		{
			return size_ == 0;
		}


		// Returns the current size of the buffer
		std::size_t Size() const
		{
			return size_;
		}

		void Clear()
		{
			head_ = 0;
			tail_ = 0;
			size_ = 0;
		}

	private:
		std::array<SpanData, Capacity> buffer_;
		std::size_t head_ = 0; // Next write
		std::size_t tail_ = 0; // Next read
		std::size_t size_ = 0;
	};


	template<typename EnumType>
	struct TracerConfigEntry
	{
		EnumType key;
		TracerConfiguration config;

		consteval TracerConfigEntry(EnumType k, TracerConfiguration c) : key(k), config(c)
		{
		}
	};

	export
	template<group_enum EnumType, TracerConfigEntry<EnumType>... Entries>
	class ProviderBuilder;

	/**
	 * @brief Entrypoint for creating Tracers
	 */
	export
	template<group_enum EnumType, typename... Configurations>
	class Provider
	{
		static constexpr auto COUNT = std::to_underlying(EnumType::COUNT);

	public:
		consteval Provider(const Provider& other)                = delete;
		consteval Provider(Provider&& other) noexcept            = delete;
		consteval Provider& operator=(const Provider& other)     = delete;
		consteval Provider& operator=(Provider&& other) noexcept = delete;

		consteval bool IsTracerEnabled(EnumType value) const
		{
			return tracers_[static_cast<std::size_t>(value)].GetConfiguration().enabled;
		}

		consteval const auto& Get(EnumType value) const
		{
			return tracers_[static_cast<std::size_t>(value)];
		}

		/**
		 * @brief Retrieves the default tracer
		 */
		consteval const auto& Get() const
		{
			return tracers_[0];
		}

		constexpr std::uint8_t Size() const
		{
			return tracers_.size();
		}

	private:
		template<group_enum T, TracerConfigEntry<T>... Entries>
		friend class ProviderBuilder;

		template<typename... TracerConfigurations>
		consteval explicit Provider(Tracer<TracerConfigurations>&&... tracers)
			: tracers_(std::forward<Tracer<TracerConfigurations>>(tracers)...)
		{
		}

		std::tuple<Tracer<Configurations>...> tracers_;

		static thread_local SpanRingBuffer<256> storage_;
	};


	/**
	 * @brief Builder for creating a Tracer Provider
	 */
	template<group_enum EnumType, TracerConfigEntry<EnumType>... Entries>
	class ProviderBuilder
	{
		// TODO: Use reflection instead of a hard-coded count type
		static constexpr auto COUNT = std::to_underlying(EnumType::COUNT);

	public:
		explicit consteval ProviderBuilder() = default;

		template<EnumType Key, TracerConfiguration Config>
		consteval auto AddConfiguration() const
		{
			return ProviderBuilder<EnumType, Entries..., TracerConfigEntry<EnumType>{Key, Config}>();
		}

		consteval auto Build() const
		{
			static_assert(
				sizeof...(Entries) == std::to_underlying(EnumType::COUNT),
				"All enum values must have a configuration.");
			return buildProvider(std::make_index_sequence<sizeof...(Entries)>{});
		}

	private:
		template<std::size_t... Is>
		consteval auto buildProvider(std::index_sequence<Is...>) const
		{
			// Helper to extract config from each entry
			return Provider<EnumType, decltype(std::get<Is>(std::tuple{Entries...}).config)...>(
				Tracer<decltype(std::get<Is>(std::tuple{Entries...}).config)>{}...
				);
		}
	};
}
