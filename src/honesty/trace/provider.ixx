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

		static constexpr std::uint8_t MAX_TRACERS = 32;

		template<typename... Names>
		explicit consteval Provider(const std::zstring_view rootName, Names... names)
			requires std::conjunction_v<std::is_convertible<Names, std::zstring_view>...>:
			tracers_{Tracer(rootName), Tracer(names)...}
		{
		}

		consteval bool ValidateName(const std::zstring_view name) const
		{
			for (const Tracer& tracer: tracers_)
			{
				if (tracer.Label() == name)
				{
					return true;
				}
			}

			return false;
		}

		consteval const Tracer& Get(const std::zstring_view name) const
		{
			static int counter = 0;
			for (const Tracer& tracer: tracers_)
			{
				if (tracer.Label() == name)
				{
					return tracer;
				}
			}
			throw std::runtime_error("Tracer with the given name not found");
		}

		constexpr std::uint8_t Size() const
		{
			return tracers_.size();
		}

	private:
		static constexpr Provider PROVIDER;

		std::inplace_vector<Tracer, MAX_TRACERS> tracers_;

		static thread_local SpanRingBuffer<256> storage_;
	};
}
