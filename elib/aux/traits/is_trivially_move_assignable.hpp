#ifndef ELIB_AUX_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_HPP
#define ELIB_AUX_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_HPP

#include <elib/config.hpp>
#include <type_traits>

namespace elib { namespace aux
{
# if !ELIB_WORKAROUND(ELIB_CONFIG_LIBSTDCXX, LIBSTDCXX_LIMITED_TYPE_TRAITS)
    using std::is_trivially_move_assignable;
        
#   if defined(ELIB_CONFIG_HAS_VARIABLE_TEMPLATES)
    template <class T>
    constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;
#   endif
# endif /* LIBSTDCXX_LIMITED_TYPE_TRAITS */
}}                                                           //namespace elib
#endif /* ELIB_AUX_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_HPP */
