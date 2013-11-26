#ifndef ELIB_MP_SEQUENCE_HAS_KEY_HPP
#define ELIB_MP_SEQUENCE_HAS_KEY_HPP

# include <elib/mp/sequence/sequence_fwd.hpp>

namespace elib
{
  namespace mp
  {
    namespace detail
    {

      //TODO
      template <class Tag>
      struct has_key_impl
      {
        template <class Seq, class Key>
        struct apply;
      };

    }                                                       // namespace detail
  }                                                         // namespace mp
}                                                           // namespace elib
#endif /* ELIB_MP_SEQUENCE_HAS_KEY_HPP */