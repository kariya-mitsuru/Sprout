#ifndef SPROUT_OPERATION_FIXED_APPEND_FRONT_HPP
#define SPROUT_OPERATION_FIXED_APPEND_FRONT_HPP

#include <cstddef>
#include <sprout/config.hpp>
#include <sprout/index_tuple.hpp>
#include <sprout/fixed_container/traits.hpp>
#include <sprout/fixed_container/functions.hpp>
#include <sprout/operation/fixed/append.hpp>

namespace sprout {
	namespace fixed {
		namespace result_of {
			//
			// append_front
			//
			template<typename Container, typename Input>
			struct append_front
				: public sprout::fixed::result_of::append<Container, Input>
			{};
		}	// namespace result_of

		//
		// append_front
		//
		template<typename Container, typename Input>
		SPROUT_CONSTEXPR inline typename sprout::fixed::result_of::append_front<Container, Input>::type append_front(
			Container const& cont,
			Input const& input
			)
		{
			return sprout::fixed::detail::append_impl<typename sprout::fixed::result_of::append_front<Container, Input>::type>(
				cont,
				typename sprout::index_range<0, sprout::fixed_container_traits<typename sprout::fixed::result_of::append_front<Container, Input>::type>::fixed_size>::type(),
				sprout::fixed_begin_offset(cont),
				sprout::size(input),
				input
				);
		}
	}	// namespace fixed

	namespace result_of {
		using sprout::fixed::result_of::append_front;
	}	// namespace result_of

	using sprout::fixed::append_front;
}	// namespace sprout

#endif	// #ifndef SPROUT_OPERATION_FIXED_APPEND_FRONT_HPP