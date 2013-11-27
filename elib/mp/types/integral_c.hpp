#ifndef ELIB_MP_TYPES_INTEGRAL_C_HPP
#define ELIB_MP_TYPES_INTEGRAL_C_HPP

# include <type_traits>

namespace elib 
{
  namespace mp
  {
    
    template <class T, T Val>
    using integral_c = std::integral_constant<T, Val>;
    
  }                                                           // namespace mp
}                                                           // namespace elib
#endif /* ELIB_MP_TYPES_INTEGRAL_C_HPP */