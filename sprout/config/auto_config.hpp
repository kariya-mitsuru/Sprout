#ifndef SPROUT_CONFIG_AUTO_CONFIG_HPP
#define SPROUT_CONFIG_AUTO_CONFIG_HPP

#include <sprout/config/compiler.hpp>

//
// SPROUT_CONFIG_DISABLE_CONSTEXPR
//
#ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR
#	ifdef SPROUT_NO_CONSTEXPR
#		define SPROUT_CONFIG_DISABLE_CONSTEXPR
#	endif	// #ifdef SPROUT_NO_CONSTEXPR
#endif	// #ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR

//
// SPROUT_CONFIG_DISABLE_NOEXCEPT
//
#ifndef SPROUT_CONFIG_DISABLE_NOEXCEPT
#	ifdef SPROUT_NO_NOEXCEPT
#		define SPROUT_CONFIG_DISABLE_NOEXCEPT
#	endif	// #ifdef SPROUT_NO_NOEXCEPT
#endif	// #ifndef SPROUT_CONFIG_DISABLE_NOEXCEPT

//
// SPROUT_CONFIG_DISABLE_TEMPLATE_ALIASES
//
#ifndef SPROUT_CONFIG_DISABLE_TEMPLATE_ALIASES
#	ifdef SPROUT_NO_TEMPLATE_ALIASES
#		define SPROUT_CONFIG_DISABLE_TEMPLATE_ALIASES
#	endif	// #ifdef SPROUT_NO_TEMPLATE_ALIASES
#endif	// #ifndef SPROUT_CONFIG_DISABLE_TEMPLATE_ALIASES

//
// SPROUT_USE_USER_DEFINED_LITERALS
//
#ifndef SPROUT_USE_USER_DEFINED_LITERALS
#	ifdef SPROUT_NO_USER_DEFINED_LITERALS
#		define SPROUT_CONFIG_DISABLE_USER_DEFINED_LITERALS
#	endif	// #ifdef SPROUT_NO_USER_DEFINED_LITERALS
#endif	// #ifndef SPROUT_USE_USER_DEFINED_LITERALS

//
// SPROUT_CONFIG_DISABLE_DELEGATING_CONSTRUCTORS
//
#ifndef SPROUT_CONFIG_DISABLE_DELEGATING_CONSTRUCTORS
#	ifdef SPROUT_NO_DELEGATING_CONSTRUCTORS
#		define SPROUT_CONFIG_DISABLE_DELEGATING_CONSTRUCTORS
#	endif	// #ifdef SPROUT_NO_DELEGATING_CONSTRUCTORS
#endif	// #ifndef SPROUT_CONFIG_DISABLE_DELEGATING_CONSTRUCTORS

//
// SPROUT_CONFIG_USE_SSCRISK_CEL
//

//
// SPROUT_CONFIG_DISABLE_BUILTIN_CMATH_FUNCTION
//
#ifndef SPROUT_CONFIG_DISABLE_BUILTIN_CMATH_FUNCTION
#	ifndef SPROUT_HAS_CONSTEXPR_CMATH_FUNCTION
#		define SPROUT_CONFIG_DISABLE_BUILTIN_CMATH_FUNCTION
#	endif	// #ifdef SPROUT_HAS_CONSTEXPR_CMATH_FUNCTION
#endif	// #ifndef SPROUT_CONFIG_DISABLE_SUPPORT_TEMPORARY_CONTAINER_ITERATION

//
// SPROUT_CONFIG_DISABLE_SUPPORT_TEMPORARY_CONTAINER_ITERATION
// SPROUT_CONFIG_SUPPORT_TEMPORARY_CONTAINER_ITERATION
//
#ifndef SPROUT_CONFIG_DISABLE_SUPPORT_TEMPORARY_CONTAINER_ITERATION
#	define SPROUT_CONFIG_SUPPORT_TEMPORARY_CONTAINER_ITERATION
#endif	// #ifndef SPROUT_CONFIG_DISABLE_SUPPORT_TEMPORARY_CONTAINER_ITERATION

#endif	// #ifndef SPROUT_CONFIG_AUTO_CONFIG_HPP