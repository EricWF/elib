#ifndef ELIB_AUX_TRAITS_IS_POINTER_HPP
#define ELIB_AUX_TRAITS_IS_POINTER_HPP

#include <elib/config.hpp>
#include <type_traits>

namespace elib { namespace aux
{
    using std::is_pointer;
        
# if defined(ELIB_CONFIG_HAS_VARIABLE_TEMPLATES)
    template <class T>
    constexpr bool is_pointer_v = is_pointer<T>::value;
# endif
}}                                                           //namespace elib
#endif /* ELIB_AUX_TRAITS_IS_POINTER_HPP */
