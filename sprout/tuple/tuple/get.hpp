/*=============================================================================
  Copyright (c) 2011-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_TUPLE_TUPLE_GET_HPP
#define SPROUT_TUPLE_TUPLE_GET_HPP

#include <utility>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/utility/move.hpp>
#include <sprout/tuple/tuple/tuple.hpp>
#include <sprout/tuple/tuple/tuple_size.hpp>
#include <sprout/tuple/tuple/tuple_element.hpp>
#include <sprout/tuple/tuple/tuple_traits.hpp>
#include <sprout/tuple/tuple/tuple_access_traits.hpp>
#include <sprout/adl/not_found.hpp>

namespace sprout {
	namespace tuples {
		//
		// tuple_get
		//
		template<std::size_t I, typename... Types>
		inline SPROUT_CONSTEXPR typename sprout::tuples::tuple_element<I, sprout::tuples::tuple<Types...> >::type&
		tuple_get(sprout::tuples::tuple<Types...>& t) SPROUT_NOEXCEPT {
			return sprout::tuples::detail::tuple_access<Types...>::template get<I>(t);
		}
		template<std::size_t I, typename... Types>
		inline SPROUT_CONSTEXPR typename sprout::tuples::tuple_element<I, sprout::tuples::tuple<Types...> >::type&&
		tuple_get(sprout::tuples::tuple<Types...>&& t) SPROUT_NOEXCEPT {
			return sprout::tuples::detail::tuple_access<Types...>::template get<I>(sprout::move(t));
		}
		template<std::size_t I, typename... Types>
		inline SPROUT_CONSTEXPR typename sprout::tuples::tuple_element<I, sprout::tuples::tuple<Types...> >::type const&
		tuple_get(sprout::tuples::tuple<Types...> const& t) SPROUT_NOEXCEPT {
			return sprout::tuples::detail::tuple_access<Types...>::template get<I>(t);
		}
	}	// namespace tuples

	using sprout::tuples::tuple_get;
}	// namespace sprout

namespace sprout_adl {
	template<std::size_t I>
	sprout::not_found_via_adl tuple_get(...);
}	// namespace sprout_adl

namespace sprout_tuple_detail {
	using sprout_adl::tuple_get;

	template<std::size_t I, typename T>
	inline SPROUT_CONSTEXPR typename sprout::tuples::tuple_traits<T>::template lvalue_reference<I>::type
	tuple_get(T& t)
	SPROUT_NOEXCEPT_IF_EXPR(sprout::tuples::tuple_access_traits<T>::template tuple_get<I>(std::declval<T&>()))
	{
		return sprout::tuples::tuple_access_traits<T>::template tuple_get<I>(t);
	}
	template<std::size_t I, typename T>
	inline SPROUT_CONSTEXPR typename std::enable_if<
		!std::is_const<T>::value && !std::is_volatile<T>::value && !std::is_reference<T>::value,
		typename sprout::tuples::tuple_traits<typename std::remove_reference<T>::type>::template rvalue_reference<I>::type
	>::type
	tuple_get(T&& t)
	SPROUT_NOEXCEPT_IF_EXPR(sprout::tuples::tuple_access_traits<typename std::remove_reference<T>::type>::template tuple_get<I>(std::declval<T&&>()))
	{
		return sprout::move(sprout::tuples::tuple_access_traits<T>::template tuple_get<I>(t));
	}
	template<std::size_t I, typename T>
	inline SPROUT_CONSTEXPR typename sprout::tuples::tuple_traits<T const>::template lvalue_reference<I>::type
	tuple_get(T const& t)
	SPROUT_NOEXCEPT_IF_EXPR(sprout::tuples::tuple_access_traits<T const>::template tuple_get<I>(std::declval<T const&>()))
	{
		return sprout::tuples::tuple_access_traits<T const>::template tuple_get<I>(t);
	}

	template<std::size_t I, typename T>
	inline SPROUT_CONSTEXPR decltype(tuple_get<I>(std::declval<T>()))
	call_tuple_get(T&& t)
	SPROUT_NOEXCEPT_IF_EXPR(tuple_get<I>(std::declval<T>()))
	{
		return tuple_get<I>(SPROUT_FORWARD(T, t));
	}
}	// namespace sprout_tuple_detail

namespace sprout {
	namespace tuples {
		//
		// get
		//
		//	effect:
		//		ADL callable tuple_get<I>(t) -> tuple_get<I>(t)
		//		otherwise -> sprout::tuples::tuple_access_traits<T>::tuple_get<I>(t)
		//		[default]
		//			std::get<I>(t)
		//
		template<std::size_t I, typename T>
		inline SPROUT_CONSTEXPR decltype(sprout_tuple_detail::call_tuple_get<I>(std::declval<T>()))
		get(T&& t)
		SPROUT_NOEXCEPT_IF_EXPR(sprout_tuple_detail::call_tuple_get<I>(std::declval<T>()))
		{
			return sprout_tuple_detail::call_tuple_get<I>(SPROUT_FORWARD(T, t));
		}
	}	// namespace tuples

	using sprout::tuples::get;
}	// namespace sprout

#endif	// #ifndef SPROUT_TUPLE_TUPLE_GET_HPP
