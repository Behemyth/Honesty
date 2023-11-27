
export module synodic.honesty.test:generator;

import std;
import :implementation;

export namespace synodic::honesty
{

	template<typename  T>
	class [[nodiscard]] generator
	{
		class promise
		{
		public:
			using reference = std::add_lvalue_reference_t<T>;
			using pointer	= std::add_pointer_t<reference>;

			auto get_return_object() noexcept
			{
				return generator(std::coroutine_handle<promise_type>::from_promise(*this));
			}

			static std::suspend_always initial_suspend() noexcept
			{
				return {};
			}

			static std::suspend_always final_suspend() noexcept
			{
				return {};
			}

			std::suspend_always yield_value(std::remove_reference_t<reference>& value) noexcept
			{
				m_value = std::addressof(value);
				return {};
			}

			std::suspend_always yield_value(std::remove_reference_t<reference>&& value) noexcept
			{
				m_value = std::addressof(value);
				return {};
			}

			std::suspend_always yield_value(generator& value) noexcept
			{
				// m_value = std::addressof(value);
				return {};
			}

			reference value() const noexcept
			{
				return *m_value;
			}

			// Don't allow any use of 'co_await' inside the generator coroutine.
			template<typename U>
			std::suspend_never await_transform(U&& value) = delete;

			static void return_void() noexcept
			{
			}

			static void unhandled_exception()
			{
				throw;
			}

		private:
			pointer m_value;
			friend generator;
		};

		struct sentinel
		{
		};

		class iterator
		{
			using coroutine_handle = std::coroutine_handle<promise>;

		public:
			using iterator_category = std::input_iterator_tag;
			using difference_type	= std::ptrdiff_t;
			using reference			= promise::reference;

			iterator() noexcept		  = default;
			iterator(const iterator&) = delete;

			iterator(iterator&& o) noexcept
			{
				std::swap(m_coroutine, o.m_coroutine);
			}

			iterator& operator=(iterator&& o) noexcept
			{
				std::swap(m_coroutine, o.m_coroutine);
				return *this;
			}

			explicit iterator(coroutine_handle coroutine) noexcept :
				m_coroutine(coroutine)
			{
			}

			~iterator()
			{
				if (m_coroutine)
				{
					m_coroutine.destroy();
				}
			}

			bool operator==(sentinel) const noexcept
			{
				return !m_coroutine || m_coroutine.done();
			}

			iterator& operator++()
			{
				m_coroutine.resume();
				return *this;
			}

			void operator++(int)
			{
				(void)operator++();
			}

			reference operator*() const noexcept
			{
				return m_coroutine.promise().value();
			}

		private:
			coroutine_handle m_coroutine = nullptr;
		};

	public:
		using promise_type = promise;

		generator(generator&& other) noexcept :
			m_coroutine(exchange(other.m_coroutine, nullptr))
		{
		}

		generator(const generator& other) = delete;

		~generator()
		{
			if (m_coroutine)
			{
				m_coroutine.destroy();
			}
		}

		generator& operator=(generator&& other) noexcept
		{
			swap(other);
			return *this;
		}

		auto begin()
		{
			m_coroutine.resume();
			return iterator {std::exchange(m_coroutine, nullptr)};
		}

		static auto end() noexcept
		{
			return sentinel {};
		}

		void swap(generator& other) noexcept
		{
			std::swap(m_coroutine, other.m_coroutine);
		}

	private:
		explicit generator(std::coroutine_handle<promise> coroutine) noexcept :
			m_coroutine(coroutine)
		{
		}

		std::coroutine_handle<promise> m_coroutine = nullptr;
	};

}
