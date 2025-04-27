export module synodic.honesty.trace:provider;

import std;

import :tracer;
import :span;
import zstring_view;
import inplace_vector;
import synodic.honesty.utility;

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

	/**
	 * @brief Entrypoint for creating Tracers
	 */
	export
	class Provider
	{
	public:
		static constexpr std::size_t MAX_TRACERS = 32;

		// TODO: Require a configuration for each EnumType, C++26
		// TODO: Constrain the enum type MAX_TRACER value
		template<typename EnumType, typename... Configuration>
		explicit consteval Provider(const Configuration&... configurations)
			requires (std::is_scoped_enum_v<EnumType> &&
			          sizeof...(Configuration) >= 1 &&
			          std::conjunction_v<std::is_same<Configuration, Tracer::Configuration>...>):
			enumTypeHash_(typeid(EnumType).hash_code()),
			tracers_{Tracer(configurations)...}
		{
		}

		Provider(const Provider& other)                = delete;
		Provider(Provider&& other) noexcept            = delete;
		Provider& operator=(const Provider& other)     = delete;
		Provider& operator=(Provider&& other) noexcept = delete;

		template<typename EnumType, EnumType EnumValue = 0>
			requires std::is_scoped_enum_v<EnumType>
		consteval bool IsTracerEnabled() const
		{
			if constexpr (typeid(EnumType).hash_code() != enumTypeHash_)
			{
				return false;
			}

			return tracers_[static_cast<std::size_t>(EnumValue)].GetConfiguration().enabled;
		}

		template<typename EnumType, EnumType EnumValue>
			requires std::is_scoped_enum_v<EnumType>
		consteval const Tracer& Get() const
		{
			if constexpr (typeid(EnumType).hash_code() != enumTypeHash_)
			{
				return tracers_[0];
			}

			return tracers_[static_cast<std::size_t>(EnumValue)];
		}

		constexpr std::uint8_t Size() const
		{
			return tracers_.size();
		}

	private:
		std::uint64_t enumTypeHash_;

		std::inplace_vector<Tracer, MAX_TRACERS> tracers_;

		static thread_local SpanRingBuffer<256> storage_;

		constinit static Provider singleton_;
	};
}
