#ifndef ELIB_MP_PROTECT_HPP
#define ELIB_MP_PROTECT_HPP

# include <elib/mp/bind.hpp>

namespace elib 
{
  namespace mp
  {
    
    // Default case: F is not a bind expression
    template <class F>
    struct protect : F
    {};
    
    
    template <class F, class ...Args>
    struct protect< bind<F, Args...> >
    {
      
      template <class ...ApplyArgs>
      using apply = typename
          bind<F, Args...>::template apply< ApplyArgs... >;
    };                                                      // struct protect
    
    
  }                                                         // namespace mp
}                                                           // namespace elib
#endif /* ELIB_MP_PROTECT_HPP */