#ifndef SPROUT_FUNCTIONAL_DFT_FIXED_DFT_ELEMENT_HPP
#define SPROUT_FUNCTIONAL_DFT_FIXED_DFT_ELEMENT_HPP

#include <iterator>
#include <sprout/config.hpp>
#include <sprout/functional/dft/detail/dft_element_gen.hpp>
#include <sprout/math/constants.hpp>
#include HDR_ITERATOR_SSCRISK_CEL_OR_SPROUT_DETAIL

namespace sprout {
	namespace detail {
		template<typename InputIterator, typename Size>
		SPROUT_CONSTEXPR inline typename std::iterator_traits<InputIterator>::value_type dft_element_impl(
			InputIterator first,
			InputIterator last,
			typename std::iterator_traits<InputIterator>::difference_type i,
			Size size
			)
		{
			typedef typename std::iterator_traits<InputIterator>::value_type value_type;
			typedef typename value_type::value_type elem_type;
			return sprout::detail::dft_element_gen(
				first,
				last,
				-(2 * sprout::math::pi<elem_type>() * i / size)
				);
		}
	}	// namespace detail
	//
	// dft_element
	//
	template<typename InputIterator>
	SPROUT_CONSTEXPR inline typename std::iterator_traits<InputIterator>::value_type dft_element(
		InputIterator first,
		InputIterator last,
		typename std::iterator_traits<InputIterator>::difference_type i
		)
	{
		return sprout::detail::dft_element_impl(
			first,
			last,
			i,
			NS_SSCRISK_CEL_OR_SPROUT_DETAIL::distance(first, last)
			);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_FUNCTIONAL_DFT_FIXED_DFT_ELEMENT_HPP