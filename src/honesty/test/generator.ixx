
export module synodic.honesty.test:generator;

import std;

////////////////////////////////////////////////////////////////
// Reference implementation of std::generator proposal D2502R0.
//
// Incorporates a suggested change to require use of ranges::elements_of(range)
// when yielding elements of a child range from a generator.
//
// This now also supports yielding an arbitrary range/view as long
// as the elements of that range are convertible to the current
// generator's reference type.

#ifdef _MSC_VER
#	define _EMPTY_BASES __declspec(empty_bases)
#	ifdef __clang__
#		define _NO_UNIQUE_ADDRESS
#	else
#		define _NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#	endif
#else
#	define _EMPTY_BASES
#	define _NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

export namespace std
{
	struct alignas(__STDCPP_DEFAULT_NEW_ALIGNMENT__) _Aligned_block
	{
		unsigned char _Pad[__STDCPP_DEFAULT_NEW_ALIGNMENT__];
	};

	template<class _Alloc>
	using _Rebind = typename allocator_traits<_Alloc>::template rebind_alloc<_Aligned_block>;

	template<class _Allocator = void>
	class _Promise_allocator
	{  // statically specified allocator type
	private:
		using _Alloc = _Rebind<_Allocator>;

		static void* _Allocate(_Alloc _Al, const size_t _Size)
		{
			if constexpr (default_initializable<_Alloc> && allocator_traits<_Alloc>::is_always_equal::value)
			{
				// do not store stateless allocator
				const size_t _Count = (_Size + sizeof(_Aligned_block) - 1) / sizeof(_Aligned_block);
				return _Al.allocate(_Count);
			}
			else
			{
				// store stateful allocator
				static constexpr size_t _Align = (::std::max)(alignof(_Alloc), sizeof(_Aligned_block));
				const size_t _Count			   = (_Size + sizeof(_Alloc) + _Align - 1) / sizeof(_Aligned_block);
				void* const _Ptr			   = _Al.allocate(_Count);
				const auto _Al_address =
					(reinterpret_cast<uintptr_t>(_Ptr) + _Size + alignof(_Alloc) - 1) & ~(alignof(_Alloc) - 1);
				::new (reinterpret_cast<void*>(_Al_address)) _Alloc(::std::move(_Al));
				return _Ptr;
			}
		}

	public:
		static void* operator new(const size_t _Size)
			requires default_initializable<_Alloc>
		{
			return _Allocate(_Alloc {}, _Size);
		}

		template<class _Alloc2, class... _Args>
			requires convertible_to<_Alloc2, _Allocator>
		static void* operator new(const size_t _Size, allocator_arg_t, _Alloc2&& _Al, _Args&...)
		{
			return _Allocate(static_cast<_Alloc>(static_cast<_Allocator>(::std::forward<_Alloc2>(_Al))), _Size);
		}

		template<class _This, class _Alloc2, class... _Args>
			requires convertible_to<_Alloc2, _Allocator>
		static void* operator new(const size_t _Size, _This&, allocator_arg_t, _Alloc2&& _Al, _Args&...)
		{
			return _Allocate(static_cast<_Alloc>(static_cast<_Allocator>(::std::forward<_Alloc2>(_Al))), _Size);
		}

		static void operator delete(void* const _Ptr, const size_t _Size) noexcept
		{
			if constexpr (default_initializable<_Alloc> && allocator_traits<_Alloc>::is_always_equal::value)
			{
				// make stateless allocator
				_Alloc _Al {};
				const size_t _Count = (_Size + sizeof(_Aligned_block) - 1) / sizeof(_Aligned_block);
				_Al.deallocate(static_cast<_Aligned_block*>(_Ptr), _Count);
			}
			else
			{
				// retrieve stateful allocator
				const auto _Al_address =
					(reinterpret_cast<uintptr_t>(_Ptr) + _Size + alignof(_Alloc) - 1) & ~(alignof(_Alloc) - 1);
				auto& _Stored_al = *reinterpret_cast<_Alloc*>(_Al_address);
				_Alloc _Al {::std::move(_Stored_al)};
				_Stored_al.~_Alloc();

				static constexpr size_t _Align = (::std::max)(alignof(_Alloc), sizeof(_Aligned_block));
				const size_t _Count			   = (_Size + sizeof(_Alloc) + _Align - 1) / sizeof(_Aligned_block);
				_Al.deallocate(static_cast<_Aligned_block*>(_Ptr), _Count);
			}
		}
	};

	template<>
	class _Promise_allocator<void>
	{  // type-erased allocator
	private:
		using _Dealloc_fn = void (*)(void*, size_t);

		template<class _Alloc2>
		static void* _Allocate(_Alloc2 _Al2, size_t _Size)
		{
			using _Alloc = _Rebind<_Alloc2>;
			auto _Al	 = static_cast<_Alloc>(_Al2);

			if constexpr (default_initializable<_Alloc> && allocator_traits<_Alloc>::is_always_equal::value)
			{
				// don't store stateless allocator
				const _Dealloc_fn _Dealloc = [](void* const _Ptr, const size_t _Size)
				{
					_Alloc _Al {};
					const size_t _Count =
						(_Size + sizeof(_Dealloc_fn) + sizeof(_Aligned_block) - 1) / sizeof(_Aligned_block);
					_Al.deallocate(static_cast<_Aligned_block*>(_Ptr), _Count);
				};

				const size_t _Count =
					(_Size + sizeof(_Dealloc_fn) + sizeof(_Aligned_block) - 1) / sizeof(_Aligned_block);
				void* const _Ptr = _Al.allocate(_Count);
				::memcpy(static_cast<char*>(_Ptr) + _Size, &_Dealloc, sizeof(_Dealloc));
				return _Ptr;
			}
			else
			{
				// store stateful allocator
				static constexpr size_t _Align = (::std::max)(alignof(_Alloc), sizeof(_Aligned_block));

				const _Dealloc_fn _Dealloc = [](void* const _Ptr, size_t _Size)
				{
					_Size += sizeof(_Dealloc_fn);
					const auto _Al_address =
						(reinterpret_cast<uintptr_t>(_Ptr) + _Size + alignof(_Alloc) - 1) & ~(alignof(_Alloc) - 1);
					auto& _Stored_al = *reinterpret_cast<const _Alloc*>(_Al_address);
					_Alloc _Al {::std::move(_Stored_al)};
					_Stored_al.~_Alloc();

					const size_t _Count = (_Size + sizeof(_Al) + _Align - 1) / sizeof(_Aligned_block);
					_Al.deallocate(static_cast<_Aligned_block*>(_Ptr), _Count);
				};

				const size_t _Count = (_Size + sizeof(_Dealloc_fn) + sizeof(_Al) + _Align - 1) / sizeof(_Aligned_block);
				void* const _Ptr	= _Al.allocate(_Count);
				::memcpy(static_cast<char*>(_Ptr) + _Size, &_Dealloc, sizeof(_Dealloc));
				_Size += sizeof(_Dealloc_fn);
				const auto _Al_address =
					(reinterpret_cast<uintptr_t>(_Ptr) + _Size + alignof(_Alloc) - 1) & ~(alignof(_Alloc) - 1);
				::new (reinterpret_cast<void*>(_Al_address)) _Alloc {::std::move(_Al)};
				return _Ptr;
			}

#if defined(__GNUC__) && !defined(__clang__)
			// avoid false positive "control reaches end of non-void function"
			__builtin_unreachable();
#endif	// defined(__GNUC__) && !defined(__clang__)
		}

	public:
		static void* operator new(const size_t _Size)
		{  // default: new/delete
			void* const _Ptr		   = ::operator new[](_Size + sizeof(_Dealloc_fn));
			const _Dealloc_fn _Dealloc = [](void* const _Ptr, const size_t _Size)
			{
				::operator delete[](_Ptr, _Size + sizeof(_Dealloc_fn));
			};
			std::memcpy(static_cast<char*>(_Ptr) + _Size, &_Dealloc, sizeof(_Dealloc_fn));
			return _Ptr;
		}

		template<class _Alloc, class... _Args>
		static void* operator new(const size_t _Size, allocator_arg_t, const _Alloc& _Al, _Args&...)
		{
			return _Allocate(_Al, _Size);
		}

		template<class _This, class _Alloc, class... _Args>
		static void* operator new(const size_t _Size, _This&, allocator_arg_t, const _Alloc& _Al, _Args&...)
		{
			return _Allocate(_Al, _Size);
		}

		static void operator delete(void* const _Ptr, const size_t _Size) noexcept
		{
			_Dealloc_fn _Dealloc;
			::memcpy(&_Dealloc, static_cast<const char*>(_Ptr) + _Size, sizeof(_Dealloc_fn));
			_Dealloc(_Ptr, _Size);
		}
	};

	namespace ranges
	{
		template<range _Rng, class _Alloc = allocator<byte>>
		class elements_of
		{
		private:
			_NO_UNIQUE_ADDRESS _Alloc _Al {};
			_Rng&& _Range;

		public:
			constexpr explicit elements_of(_Rng&& _Range_) noexcept(is_nothrow_default_constructible_v<_Alloc>)
				requires default_initializable<_Alloc>
				:
				_Range(::std::forward<_Rng>(_Range_))
			{
			}

			constexpr explicit elements_of(_Rng&& _Range_, _Alloc _Al_) noexcept :
				_Al(::std::move(_Al_)),
				_Range(::std::forward<_Rng>(_Range_))
			{
			}

			constexpr elements_of(elements_of&&) = default;

			constexpr _Rng&& range() noexcept
			{
				return ::std::forward<_Rng>(_Range);
			}

			constexpr _Alloc get_allocator() const noexcept
			{
				return _Al;
			}
		};

		template<class _Rng, class _Alloc = allocator<byte>>
		elements_of(_Rng&&, _Alloc = {}) -> elements_of<_Rng, _Alloc>;
	}  // namespace ranges

	template<class _Ty, class _Alloc = void, class _Uty = void>
	class generator;

	template<class _Ty, class _Uty>
	using _Generator_value_t = conditional_t<is_void_v<_Uty>, remove_cvref_t<_Ty>, _Uty>;
	template<class _Ty, class _Uty>
	using _Generator_reference_t =
		conditional_t<is_void_v<_Uty>, conditional_t<is_reference_v<_Ty>, _Ty, const _Ty&>, _Ty>;

	template<class _Ref>
	using _Generator_yield_t = conditional_t<is_reference_v<_Ref>, _Ref, const _Ref&>;

	template<class _Yielded>
	class _Generator_promise_base
	{
	public:
		static_assert(is_reference_v<_Yielded>);

		suspend_always initial_suspend() noexcept
		{
			return {};
		}

		[[nodiscard]] auto final_suspend() noexcept
		{
			return _Final_awaiter {};
		}

		[[nodiscard]] suspend_always yield_value(_Yielded _Val) noexcept
		{
			_Ptr = ::std::addressof(_Val);
			return {};
		}

		template<class _Ty, class _Alloc, class _Uty, class _Alloc2>
			requires same_as<_Generator_yield_t<_Generator_reference_t<_Ty, _Uty>>, _Yielded>
		[[nodiscard]] auto yield_value(::std::ranges::elements_of<generator<_Ty, _Alloc, _Uty>, _Alloc2> _Elem) noexcept
		{
			return _Nested_awaitable<_Ty, _Alloc, _Uty> {_Elem.range()};
		}

		template<::std::ranges::input_range _Rng, class _Alloc>
			requires convertible_to<::std::ranges::range_reference_t<_Rng>, _Yielded>
		[[nodiscard]] auto yield_value(::std::ranges::elements_of<_Rng, _Alloc> _Elem) noexcept
		{
			using _Ty	  = ::std::ranges::range_value_t<_Rng>;
			auto&& _Range = _Elem.range();
			return _Nested_awaitable<_Yielded, _Alloc, _Ty> {
				[](allocator_arg_t, _Alloc, auto* _Range_ptr) -> generator<_Yielded, _Alloc, _Ty>
				{
					for (auto&& e: *_Range_ptr)
					{
						co_yield static_cast<_Yielded>(::std::forward<decltype(e)>(e));
					}
				}(allocator_arg, _Elem.get_allocator(), ::std::addressof(_Range))};
		}

		void await_transform() = delete;

		void return_void() noexcept
		{
		}

		void unhandled_exception()
		{
			if (_Info)
			{
				_Info->_Except = ::std::current_exception();
			}
			else
			{
				throw;
			}
		}

	private:
		struct _Nest_info
		{
			exception_ptr _Except;
			coroutine_handle<_Generator_promise_base> _Parent;
			coroutine_handle<_Generator_promise_base> _Root;
		};

		struct _Final_awaiter
		{
			[[nodiscard]] bool await_ready() noexcept
			{
				return false;
			}

			template<class _Promise>
			[[nodiscard]] coroutine_handle<> await_suspend(coroutine_handle<_Promise> _Handle) noexcept
			{
#ifdef __cpp_lib_is_pointer_interconvertible
				static_assert(is_pointer_interconvertible_base_of_v<_Generator_promise_base, _Promise>);
#endif	// __cpp_lib_is_pointer_interconvertible

				_Generator_promise_base& _Current = _Handle.promise();
				if (!_Current._Info)
				{
					return ::std::noop_coroutine();
				}

				coroutine_handle<_Generator_promise_base> _Cont = _Current._Info->_Parent;
				_Current._Info->_Root.promise()._Top			= _Cont;
				_Current._Info									= nullptr;
				return _Cont;
			}

			void await_resume() noexcept
			{
			}
		};

		template<class _Ty, class _Alloc, class _Uty>
		struct _Nested_awaitable
		{
			static_assert(same_as<_Generator_yield_t<_Generator_reference_t<_Ty, _Uty>>, _Yielded>);

			_Nest_info _Nested;
			generator<_Ty, _Alloc, _Uty> _Gen;

			explicit _Nested_awaitable(generator<_Ty, _Alloc, _Uty>&& _Gen_) noexcept :
				_Gen(::std::move(_Gen_))
			{
			}

			[[nodiscard]] bool await_ready() noexcept
			{
				return !_Gen._Coro;
			}

			template<class _Promise>
			[[nodiscard]] coroutine_handle<_Generator_promise_base>
				await_suspend(coroutine_handle<_Promise> _Current) noexcept
			{
#ifdef __cpp_lib_is_pointer_interconvertible
				static_assert(is_pointer_interconvertible_base_of_v<_Generator_promise_base, _Promise>);
#endif	// __cpp_lib_is_pointer_interconvertible
				auto _Target	= coroutine_handle<_Generator_promise_base>::from_address(_Gen._Coro.address());
				_Nested._Parent = coroutine_handle<_Generator_promise_base>::from_address(_Current.address());
				_Generator_promise_base& _Parent_promise = _Nested._Parent.promise();
				if (_Parent_promise._Info)
				{
					_Nested._Root = _Parent_promise._Info->_Root;
				}
				else
				{
					_Nested._Root = _Nested._Parent;
				}
				_Nested._Root.promise()._Top = _Target;
				_Target.promise()._Info		 = ::std::addressof(_Nested);
				return _Target;
			}

			void await_resume()
			{
				if (_Nested._Except)
				{
					::std::rethrow_exception(::std::move(_Nested._Except));
				}
			}
		};

		template<class, class>
		friend class _Generator_iterator;

		// _Top and _Info are mutually exclusive, and could potentially be merged.
		coroutine_handle<_Generator_promise_base> _Top = coroutine_handle<_Generator_promise_base>::from_promise(*this);
		add_pointer_t<_Yielded> _Ptr				   = nullptr;
		_Nest_info* _Info							   = nullptr;
	};

	template<class _Value, class _Ref>
	class _Generator_iterator
	{
	public:
		using value_type	  = _Value;
		using difference_type = ptrdiff_t;

		_Generator_iterator(_Generator_iterator&& _That) noexcept :
			_Coro {::std::exchange(_That._Coro, {})}
		{
		}

		_Generator_iterator& operator=(_Generator_iterator&& _That) noexcept
		{
			_Coro = ::std::exchange(_That._Coro, {});
			return *this;
		}

		[[nodiscard]] _Ref operator*() const noexcept
		{
			return static_cast<_Ref>(*_Coro.promise()._Top.promise()._Ptr);
		}

		_Generator_iterator& operator++()
		{
			_Coro.promise()._Top.resume();
			return *this;
		}

		void operator++(int)
		{
			++*this;
		}

		[[nodiscard]] bool operator==(default_sentinel_t) const noexcept
		{
			return _Coro.done();
		}

	private:
		template<class, class, class>
		friend class generator;

		explicit _Generator_iterator(
			coroutine_handle<_Generator_promise_base<_Generator_yield_t<_Ref>>> _Coro_) noexcept :
			_Coro {_Coro_}
		{
		}

		coroutine_handle<_Generator_promise_base<_Generator_yield_t<_Ref>>> _Coro;
	};

	template<class _Ty, class _Alloc, class _Uty>
	class generator
	{
	public:
		using _Value = _Generator_value_t<_Ty, _Uty>;
		static_assert(
			same_as<remove_cvref_t<_Value>, _Value> && is_object_v<_Value>,
			"generator's value type must be a cv-unqualified object type");

		using _Ref = _Generator_reference_t<_Ty, _Uty>;
		static_assert(
			is_reference_v<_Ref> || (is_object_v<_Ref> && same_as<remove_cv_t<_Ref>, _Ref> && copy_constructible<_Ref>),
			"generator's second argument must be a reference type or a cv-unqualified "
			"copy-constructible object type");

		using _RRef = conditional_t<is_lvalue_reference_v<_Ref>, remove_reference_t<_Ref>&&, _Ref>;

		static_assert(
			common_reference_with<_Ref&&, _Value&> && common_reference_with<_Ref&&, _RRef&&> &&
				common_reference_with<_RRef&&, const _Value&>,
			"an iterator with the selected value and reference types cannot model indirectly_readable");

		struct _EMPTY_BASES promise_type : _Promise_allocator<_Alloc>, _Generator_promise_base<_Generator_yield_t<_Ref>>
		{
			[[nodiscard]] generator get_return_object() noexcept
			{
				return generator(coroutine_handle<promise_type>::from_promise(*this));
			}
		};

		static_assert(is_standard_layout_v<promise_type>);
#ifdef __cpp_lib_is_pointer_interconvertible
		static_assert(
			is_pointer_interconvertible_base_of_v<_Generator_promise_base<_Generator_yield_t<_Ref>>, promise_type>);
#endif	// __cpp_lib_is_pointer_interconvertible

		generator(generator&& _That) noexcept :
			_Coro(::std::exchange(_That._Coro, {}))
		{
		}

		~generator()
		{
			if (_Coro)
			{
				_Coro.destroy();
			}
		}

		generator& operator=(generator&& _That) noexcept
		{
			if (auto _Old = ::std::exchange(_Coro, ::std::exchange(_That._Coro, {})))
			{
				_Old.destroy();
			}
			return *this;
		}

		[[nodiscard]] _Generator_iterator<_Value, _Ref> begin()
		{
			// Pre: _Coro is suspended at its initial suspend point
			_Coro.resume();
			return _Generator_iterator<_Value, _Ref> {
				coroutine_handle<_Generator_promise_base<_Generator_yield_t<_Ref>>>::from_address(_Coro.address())};
		}

		[[nodiscard]] default_sentinel_t end() const noexcept
		{
			return default_sentinel;
		}

	private:
		explicit generator(coroutine_handle<promise_type> _Coro_) noexcept :
			_Coro(_Coro_)
		{
		}

		friend _Generator_promise_base<_Generator_yield_t<_Ref>>;

		coroutine_handle<promise_type> _Coro = nullptr;
	};

	namespace ranges
	{
		template<class _Ty, class _Alloc, class _Uty>
		inline constexpr bool enable_view<generator<_Ty, _Alloc, _Uty>> = true;
	}
}  // namespace std
