#ifndef SPROUT_WORKAROUND_RECURSIVE_FUNCTION_TEMPLATE_HPP
#define SPROUT_WORKAROUND_RECURSIVE_FUNCTION_TEMPLATE_HPP

#include <sprout/config.hpp>
#include <sprout/type_traits/enabler_if.hpp>
#include <sprout/preprocessor/stringize.hpp>

//
// SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_LIMIT
//
#ifndef SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_LIMIT
#	define SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_LIMIT 512
#endif

//
// SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE_DECL
// SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK_DECL
//
#define SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE_DECL(depth) \
	typename sprout::enabler_if<((depth) < SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_LIMIT - 1)>::type
#define SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK_DECL(depth) \
	typename sprout::enabler_if<((depth) >= SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_LIMIT - 1)>::type

//
// SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE
// SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK
//
#define SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE(depth) \
	SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE_DECL(depth) = sprout::enabler
#define SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK(depth) \
	SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK_DECL(depth) = sprout::enabler

//
// SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_EXCEEDED_MESSAGE
//
#define SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_EXCEEDED_MESSAGE \
	"recursive template instantiation exceeded maximum depth of " SPROUT_PP_STRINGIZE(SPROUT_RECURSIVE_FUNCTION_TEMPLATE_INSTANTIATION_LIMIT)

#endif	// #ifndef SPROUT_WORKAROUND_RECURSIVE_FUNCTION_TEMPLATE_HPP
