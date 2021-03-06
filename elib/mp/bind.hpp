#ifndef ELIB_MP_BIND_HPP
#define ELIB_MP_BIND_HPP

# include <elib/mp/config.hpp>
# include <elib/mp/arg.hpp>
# include <elib/mp/apply_wrap.hpp>

# include <cstddef>

namespace elib 
{
  namespace mp
  {
    
    template <class F, class ...Args> struct bind;
    
    namespace detail
    {
      
      //---------------------- resolve_bind_arg ------------------------------// 
      
      template <typename T, typename ...Args>
      struct resolve_bind_arg
      { using type = T; };
      
      // bind<...>
      template <typename F, typename ...Args1, typename ...Args2>
      struct resolve_bind_arg<bind<F, Args1...>, Args2...>
      {
        using type = 
          apply_wrap_t<
            bind<F, Args1...>
            , Args2...
          >;
      };
      
      // arg<N>
      template <std::size_t N, typename ...Args>
      struct resolve_bind_arg<arg<N>, Args...>
      {
        using type = apply_wrap_t<arg<N>, Args...>;
      };
      
      //--------------------- resolve_bind_arg_t ----------------------------// 
      
      template <typename ...Args>
      using resolve_bind_arg_t =
        typename resolve_bind_arg<Args...>::type;
      
      
    }                                                       // namespace detail
    
    
    template <class F, class ...Args>
    struct bind
    {
      template <class ...ApplyArgs>
      using apply = 
        apply_wrap<
            detail::resolve_bind_arg_t<F, ApplyArgs...>
          , detail::resolve_bind_arg_t<Args, ApplyArgs...>...
          >;
    };                                                      // struct bind
    
  }                                                         // namespace mp
}                                                           // namespace elib
#endif /* ELIB_MP_BIND_HPP */