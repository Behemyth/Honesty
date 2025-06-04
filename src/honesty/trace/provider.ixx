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


	export
	template<group_enum T>
	class ProviderBuilder;

	/**
	 * @brief Entrypoint for creating Tracers
	 */
	export
	template<group_enum EnumType>
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

		consteval const Tracer& Get(EnumType value) const
		{
			return tracers_[static_cast<std::size_t>(value)];
		}

		/**
		 * @brief Retrieves the default tracer
		 */
		consteval const Tracer& Get() const
		{
			return tracers_[0];
		}

		constexpr std::uint8_t Size() const
		{
			return tracers_.size();
		}

	private:
		template<group_enum T>
		friend class ProviderBuilder;

		template<typename... Tracers>
		consteval explicit Provider(Tracers&&... tracers)
			requires (sizeof...(Tracers) == COUNT) &&
			         (std::same_as<std::remove_cvref_t<Tracers>, Tracer> && ...)
			: tracers_(std::forward<Tracers>(tracers)...)
		{
		}

		std::array<Tracer, COUNT> tracers_;

		static thread_local SpanRingBuffer<256> storage_;
	};


	/**
	 * @brief Builder for creating a Tracer Provider
	 */
	template<group_enum EnumType, typename... TracerConfigurations>
	class ProviderBuilder
	{
		// TODO: Use reflection instead of a hard-coded count type
		static constexpr auto COUNT = std::to_underlying(EnumType::COUNT);

	public:
		explicit consteval ProviderBuilder(const bool enabled) :
			enabled_(enabled)
		{
		}

		template<EnumType Value>
		consteval auto AddConfiguration(const TracerConfiguration& config) const
		{
			auto newConfigs                             = configurations_;
			newConfigs[static_cast<std::size_t>(Value)] = config;
			return ProviderBuilder(enabled_, newConfigs);
		}

		consteval Provider<EnumType> Build() const
		{
			// Ensure all configurations are set at compile time
			for (std::size_t i = 0; i < COUNT; ++i)
			{
				if (!configurations_[i].has_value())
				{
					throw "All configurations must be set at compile time!";
				}
			}

			auto createProvider = [&]<std::size_t... I>(std::index_sequence<I...>) consteval
			{
				return Provider<EnumType>(Tracer(configurations_[I].value())...);
			};

			return createProvider(std::make_index_sequence<COUNT>{});
		}

	private:
		bool enabled_;
		std::array<std::optional<TracerConfiguration>, COUNT> configurations_;

		// Add a private constructor to allow AddConfiguration to return a new builder
		consteval ProviderBuilder(const bool enabled, std::array<std::optional<TracerConfiguration>, COUNT> configs)
			: enabled_(enabled), configurations_(configs)
		{
		}
	};
}
