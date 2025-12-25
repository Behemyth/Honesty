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
		void Push(const SpanData& span)
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
		SpanData Pop()
		{
			SpanData span = buffer_[tail_];
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

		// Get all spans as a contiguous view (for export)
		std::span<const SpanData> View() const
		{
			// Note: This only works correctly when buffer hasn't wrapped
			// For wrapped buffers, would need to return two spans or copy
			if (head_ >= tail_)
			{
				return std::span{buffer_.data() + tail_, size_};
			}
			// Wrapped case - would need different handling
			return std::span{buffer_.data() + tail_, Capacity - tail_};
		}

	private:
		std::array<SpanData, Capacity> buffer_;
		std::size_t head_ = 0; // Next write
		std::size_t tail_ = 0; // Next read
		std::size_t size_ = 0;
	};


	/**
	 * @brief Provider for managing multiple named tracers with shared span storage.
	 *
	 * All tracers respect the global TracingEnabled configuration. When tracing
	 * is disabled, the entire Provider compiles to minimal overhead.
	 *
	 * @tparam EnumType An enum with a COUNT member defining the tracer categories
	 */
	export template<group_enum EnumType>
	class Provider
	{
		static constexpr auto COUNT = std::to_underlying(EnumType::COUNT);

	public:
		constexpr Provider() = default;

		Provider(const Provider& other)                = delete;
		Provider(Provider&& other) noexcept            = delete;
		Provider& operator=(const Provider& other)     = delete;
		Provider& operator=(Provider&& other) noexcept = delete;

		/**
		 * @brief Check if tracing is enabled at compile time
		 */
		static consteval bool IsEnabled()
		{
			return TracingEnabled;
		}

		/**
		 * @brief Get a tracer for the specified category
		 */
		template<EnumType Value>
		const Tracer& Get() const
		{
			return tracers_[std::to_underlying(Value)];
		}

		/**
		 * @brief Get a mutable tracer for the specified category
		 */
		template<EnumType Value>
		Tracer& GetMutable()
		{
			return tracers_[std::to_underlying(Value)];
		}

		/**
		 * @brief Get the number of tracer categories
		 */
		static constexpr std::size_t Size()
		{
			return COUNT;
		}

		/**
		 * @brief Record a completed span to the buffer
		 */
		void RecordSpan(const SpanData& data)
		{
			if constexpr (TracingEnabled)
			{
				storage_.Push(data);
			}
		}

		/**
		 * @brief Drain all buffered spans and pass to the provided callback
		 * @param callback Function to receive the spans for export
		 */
		template<typename Callback>
		void DrainSpans(Callback&& callback)
		{
			if constexpr (TracingEnabled)
			{
				std::vector<SpanData> spans;
				spans.reserve(storage_.Size());

				while (!storage_.Empty())
				{
					spans.push_back(storage_.Pop());
				}

				if (!spans.empty())
				{
					callback(std::span<const SpanData>{spans});
				}
			}
		}

		/**
		 * @brief Get the number of buffered spans
		 */
		std::size_t BufferedSpanCount() const
		{
			if constexpr (TracingEnabled)
			{
				return storage_.Size();
			}
			else
			{
				return 0;
			}
		}

	private:
		std::array<Tracer, COUNT> tracers_{};

		// Only allocate storage when tracing is enabled
		[[no_unique_address]] std::conditional_t<TracingEnabled, SpanRingBuffer<256>, std::monostate> storage_{};
	};
}
