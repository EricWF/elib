#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "elib/mp/arithmetic.hpp"
#include "elib/mp/bitwise.hpp"
#include "elib/mp/logical.hpp"
#include "elib/mp/comparison.hpp"
#include "elib/mp/void.hpp"
#include "elib/mp/integral_constant.hpp"
#include "elib/pp/overload.hpp"
#include "mp_test_helper.hpp"


#define V_(x) x::type::value 
#
#define EXPAND_EXPR(...) ELIB_PP_OVERLOAD(EXPAND_EXPR_, __VA_ARGS__)(__VA_ARGS__)
#define EXPAND_EXPR_1(a) OP V_(a)
#define EXPAND_EXPR_2(a1, a2) V_(a1) OP V_(a2)
#define EXPAND_EXPR_3(a1, a2, a3) V_(a1) OP V_(a2) OP V_(a3)
#define EXPAND_EXPR_4(a1, a2, a3, a4) V_(a1) OP V_(a2) OP V_(a3) OP V_(a4)
#define EXPAND_EXPR_5(a1, a2, a3, a4, a5) V_(a1) OP V_(a2) OP V_(a3) OP V_(a4) OP V_(a5)
#define EXPAND_EXPR_6(a1, a2, a3, a4, a5, a6) V_(a1) OP V_(a2) OP V_(a3) OP V_(a4) OP V_(a5) OP V_(a6)
#
#
#define CHECK_RESULT(R, CT, result)                                       \
  CHECK( std::is_same<typename R::type::value_type, CT>::type::value ); \
  CHECK( R::type::value == result )
#
#
#define TEST_OP(etype, eval, ...)                    \
  do {                                               \
    using R = FN<__VA_ARGS__>;                       \
    using CT = decltype( EXPAND_EXPR(__VA_ARGS__) ); \
    constexpr CT result = EXPAND_EXPR(__VA_ARGS__);  \
    CHECK_RESULT(R, CT, result);                     \
    CHECK_RESULT(R, etype, eval);                    \
  } while (false)
#
#
#define TEST_SIGNED_TYPES(e, x, y)       \
  TEST_OP(int, e, char_<x>, char_<y>);   \
  TEST_OP(int, e, short_<x>, short_<y>); \
  TEST_OP(int, e, int_<x>, int_<y>);     \
  TEST_OP(long, e, long_<x>, long_<y>);  \
  TEST_OP(long long, e, llong_<x>, llong_<y>)
#
#
#define TEST_UNSIGNED_TYPES(e, x, y)               \
  TEST_OP(unsigned, e, uint_<x>, uint_<y>);        \
  TEST_OP(unsigned long, e, ulong_<x>, ulong_<y>); \
  TEST_OP(unsigned long long, e, ullong_<x>, ullong_<y>)
#
#
#define TEST_BOOL(e, x, y)  \
  TEST_OP(int, e, bool_<x>, bool_<y>)
#
#
#define TEST_TYPES(e, x, y)   \
  TEST_SIGNED_TYPES(e, x, y); \
  TEST_UNSIGNED_TYPES(e, x, y)
#
#
#define TEST_ALL_TYPES(e, x, y) \
  TEST_BOOL(e, x, y);           \
  TEST_TYPES(e, x, y)                                   
#
#
#define TEST_CONVERSIONS(e, x, y)                        \
  /* bool */                                             \
  TEST_OP(int, e, bool_<x>, char_<y>);                   \
  TEST_OP(int, e, short_<x>, bool_<y>);                  \
  TEST_OP(int, e, bool_<x>, int_<y>);                    \
  TEST_OP(unsigned, e, uint_<x>, bool_<y>);              \
  TEST_OP(long, e, bool_<x>, long_<y>);                  \
  TEST_OP(unsigned long, e, ulong_<x>, bool_<y>);        \
  TEST_OP(long long, e, bool_<x>, llong_<y>);            \
  TEST_OP(unsigned long long, e, ullong_<x>, bool_<y>);  \
  /* char */                                             \
  TEST_OP(int, e, short_<x>, char_<y>);                  \
  TEST_OP(int, e, char_<x>, int_<y>);                    \
  TEST_OP(unsigned, e, uint_<x>, char_<y>);              \
  TEST_OP(long, e, char_<x>, long_<y>);                  \
  TEST_OP(unsigned long, e, ulong_<x>, char_<y>);        \
  TEST_OP(long long, e, char_<x>, llong_<y>);            \
  TEST_OP(unsigned long long, e, ullong_<x>, char_<y>);  \
  /* short */                                            \
  TEST_OP(int, e, short_<x>, int_<y>);                   \
  TEST_OP(unsigned, e, uint_<x>, short_<y>);             \
  TEST_OP(long, e, short_<x>, long_<y>);                 \
  TEST_OP(unsigned long, e, ulong_<x>, short_<y>);       \
  TEST_OP(long long, e, short_<x>, llong_<y>);           \
  TEST_OP(unsigned long long, e, ullong_<x>, short_<y>); \
  /* int */                                              \
  TEST_OP(unsigned, e, uint_<x>, int_<y>);               \
  TEST_OP(long, e, int_<x>, long_<y>);                   \
  TEST_OP(unsigned long, e, ulong_<x>, int_<y>);         \
  TEST_OP(long long, e, int_<x>, llong_<y>);             \
  TEST_OP(unsigned long long, e, ullong_<x>, int_<y>);   \
  /* uint */                                             \
  TEST_OP(long, e, uint_<x>, long_<y>);                  \
  TEST_OP(unsigned long, e, ulong_<x>, uint_<y>);        \
  TEST_OP(long long, e, uint_<x>, llong_<y>);            \
  TEST_OP(unsigned long long, e, ullong_<x>, uint_<y>);  \
  /* long */                                             \
  TEST_OP(unsigned long, e, ulong_<x>, long_<y>);        \
  TEST_OP(long long, e, long_<x>, llong_<y>);            \
  TEST_OP(unsigned long long, e, ullong_<x>, long_<y>);  \
  /* unsigned long */                                    \
  TEST_OP(unsigned long long, e, ulong_<x>, llong_<y>);  \
  TEST_OP(unsigned long long, e, ullong_<x>, ulong_<y>)
#


BOOST_AUTO_TEST_SUITE(mp_integral_metafunctions_test_suite)

////////////////////////////////////////////////////////////////////////////////
//                            arithmetic                                              
////////////////////////////////////////////////////////////////////////////////

#define OP -
#define FN negate_t

  BOOST_AUTO_TEST_CASE(mp_arithmetic_negate)
  {
    TEST_OP(int, 0, false_);
    TEST_OP(int, -1, true_);
    TEST_OP(int, 0, int_<0>);
    TEST_OP(int, -1, int_<1>);
    TEST_OP(int, 1, int_<-1>);
    TEST_OP(unsigned, 0u, uint_<0>);
    TEST_OP(unsigned, -1u, uint_<1>);
    TEST_OP(long, 0, long_<0>);
    TEST_OP(long, 1, long_<-1>);
    TEST_OP(long, -1, long_<1>);
  }
  
#undef OP
#undef FN

#define OP +
#define FN add_t
    
  BOOST_AUTO_TEST_CASE(mp_arithmetic_add)
  {
    // all (w. bool)
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(1, 1, 0);
    TEST_ALL_TYPES(1, 0, 1);
    // all (w/o bool)
    TEST_TYPES(2, 1, 1);
    TEST_TYPES(2, 2, 0);
    TEST_TYPES(2, 0, 2);
    // signed
    TEST_SIGNED_TYPES(0, -1, 1);
    TEST_SIGNED_TYPES(0, 1, -1);
    TEST_SIGNED_TYPES(-2, -1, -1);
    // mixed
    TEST_CONVERSIONS(0, 0, 0);
  }                                                         // mp_arithmetic_add

#undef OP
#undef FN

#define OP -
#define FN subtract_t
  
  BOOST_AUTO_TEST_CASE(mp_arithmetic_subtract)
  {
    // all (w. bool)
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(1, 1, 0);
    TEST_ALL_TYPES(0, 1, 1);
    // all (w/o bool)
    TEST_TYPES(0, 2, 2);
    TEST_TYPES(1, 2, 1);
    TEST_TYPES(2, 4, 2);
    // signed
    TEST_SIGNED_TYPES(0, -1, -1);
    TEST_SIGNED_TYPES(2, 1, -1);
    TEST_SIGNED_TYPES(-2, -1, 1);
    // mixed
    TEST_CONVERSIONS(0, 0, 0);
  }                                                   // mp_arithmetic_subtract

#undef OP
#undef FN
  
#define OP *
#define FN multiply

  BOOST_AUTO_TEST_CASE(mp_arithmetic_multiply)
  {
    // all (w. bool)
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(0, 1, 0);
    TEST_ALL_TYPES(0, 0, 1);
    TEST_ALL_TYPES(1, 1, 1);
    // all (w/o bool)
    TEST_TYPES(2, 1, 2);
    TEST_TYPES(2, 2, 1);
    // signed
    TEST_SIGNED_TYPES(-1, -1, 1);
    TEST_SIGNED_TYPES(-1, 1, -1);
    TEST_SIGNED_TYPES(1, -1, -1);
    // mixed
    TEST_CONVERSIONS(0, 0, 0);
  }                                                   // mp_arithmetic_multiply
  
#undef OP
#undef FN

#define OP /
#define FN divide_t

  BOOST_AUTO_TEST_CASE(mp_arithmetic_divide)
  {
    // all (including bool)
    TEST_ALL_TYPES(0, 0, 1);
    TEST_ALL_TYPES(1, 1, 1);
    // all (no bool)
    TEST_TYPES(2, 2, 1);
    TEST_TYPES(2, 4, 2);
    // signed 
    TEST_SIGNED_TYPES(-1, 1, -1);
    TEST_SIGNED_TYPES(-1, -1, 1);
    TEST_SIGNED_TYPES(1, -1, -1);
    // mixed
    TEST_CONVERSIONS(0, 0, 1);
    TEST_CONVERSIONS(1, 1, 1);
  }                                                     // mp_arithmetic_divide
  
#undef OP
#undef FN

#define OP %
#define FN modulus_t

  BOOST_AUTO_TEST_CASE(mp_arithmetic_modulus)
  {
    TEST_ALL_TYPES(0, 0, 1);
    TEST_ALL_TYPES(0, 1, 1);
    TEST_TYPES(1, 1, 2);
    TEST_SIGNED_TYPES(1, 1, -2);
    TEST_SIGNED_TYPES(-1, -1, 2);
    TEST_SIGNED_TYPES(-1, -1, -2);
    TEST_CONVERSIONS(0, 0, 1);
    TEST_CONVERSIONS(0, 1, 1);
  }                                                    // mp_arithmetic_modulus
  
#undef OP
#undef FN 

////////////////////////////////////////////////////////////////////////////////
//                            BITWISE                                              
////////////////////////////////////////////////////////////////////////////////

#define OP ~
#define FN bitnegate_

  BOOST_AUTO_TEST_CASE(mp_bitwise_negate)
  {
    TEST_OP(int, ~0, false_);
    TEST_OP(int, ~1, true_);
    TEST_OP(int, ~0, int_<0>);
    TEST_OP(int, ~(-1), int_<-1>);
    TEST_OP(int, ~2, int_<2>);
    TEST_OP(unsigned, ~0u, uint_<0>);
    TEST_OP(unsigned, ~1u, uint_<1>);
  }                                                        // mp_bitwise_negate
  
#undef OP
#undef FN

#define OP &
#define FN bitand_
  
  BOOST_AUTO_TEST_CASE(mp_bitwise_and)
  {
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(0, 1, 0);
    TEST_ALL_TYPES(0, 0, 1);
    TEST_ALL_TYPES(1, 1, 1);
    // mixed
    TEST_CONVERSIONS(0, 0, 0);
    TEST_CONVERSIONS(1, 1, 1);
  }                                                         // mp_bitwise_and

#undef OP
#undef FN

#define OP |
#define FN bitor_

  BOOST_AUTO_TEST_CASE(mp_bitwise_or)
  {
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(1, 1, 0);
    TEST_ALL_TYPES(1, 0, 1);
    TEST_ALL_TYPES(1, 1, 1);
    // mixed
    TEST_CONVERSIONS(0, 0, 0);
    TEST_CONVERSIONS(1, 1, 1);
  }                                                         // mp_bitwise_or
  
#undef OP
#undef FN

#define OP ^
#define FN bitxor_

  BOOST_AUTO_TEST_CASE(mp_bitwise_xor)
  {
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(1, 1, 0);
    TEST_ALL_TYPES(1, 0, 1);
    TEST_ALL_TYPES(0, 1, 1);
    // mixed
    TEST_CONVERSIONS(0, 0, 0);
    TEST_CONVERSIONS(1, 1, 0);
    TEST_CONVERSIONS(0, 1, 1);
  }                                                         // mp_bitwise_xor
  
#undef OP
#undef FN

#define OP << 
#define FN shift_left
  
  BOOST_AUTO_TEST_CASE(mp_bitwise_shift_left)
  {
    // all (w. bool)
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(0, 0, 1);
    // all (w/o bool)
    TEST_TYPES(2, 1, 1);
    TEST_TYPES(4, 1, 2);
    TEST_TYPES(4, 2, 1);
  }                                                    // mp_bitwise_shift_left
  
#undef OP
#undef FN

#define OP >>
#define FN shift_right
  
  BOOST_AUTO_TEST_CASE(mp_bitwise_shift_right)
  {
    TEST_ALL_TYPES(0, 0, 0);
    TEST_ALL_TYPES(0, 0, 1);
    TEST_TYPES(1, 2, 1);
    TEST_TYPES(1, 4, 2);
    TEST_TYPES(2, 4, 1);
  }                                                   // mp_bitwise_shift_right

#undef OP
#undef FN

////////////////////////////////////////////////////////////////////////////////
//                            LOGICAL                                              
////////////////////////////////////////////////////////////////////////////////


  BOOST_AUTO_TEST_CASE(mp_logical_not)
  {
    SAME_TYPE(false_, not_<true_>);
    SAME_TYPE(true_, not_<false_>);
    SAME_TYPE(true_, not_<int_<0>>);
    SAME_TYPE(true_, not_<ullong_<0>>);
    SAME_TYPE(false_, not_<char_<1>>);
    SAME_TYPE(false_, not_<short_<-1>>);
    // or_c
    SAME_TYPE(false_, not_c<true>);
    SAME_TYPE(true_, not_c<false>);
    SAME_TYPE(true_, not_c<0>);
    SAME_TYPE(true_, not_c<0u>);
    SAME_TYPE(true_, not_c<0l>);
    SAME_TYPE(true_, not_c<static_cast<unsigned long long>(0)>);
    SAME_TYPE(false_, not_c<1>);
    SAME_TYPE(false_, not_c<static_cast<unsigned>(-1)>);
  }                                                         // mp_logical_not
  
  BOOST_AUTO_TEST_CASE(mp_logical_and)
  {
    SAME_TYPE(true_, and_<true_, true_>);
    SAME_TYPE(true_, and_<true_, true_, true_>);
    SAME_TYPE(false_, and_<true_, false_>);
    SAME_TYPE(false_, and_<false_, true_>);
    SAME_TYPE(false_, and_<false_, false_>);
    SAME_TYPE(false_, and_<true_, true_, false_>);
    // short circuit test
    SAME_TYPE(false_, and_<true_, false_, void_>);
    // and_c
    SAME_TYPE(true_, and_c<true, true>);
    SAME_TYPE(true_, and_c<true, true, true>);
    SAME_TYPE(false_, and_c<true, false>);
    SAME_TYPE(false_, and_c<false, true>);
    SAME_TYPE(false_, and_c<false, false>);
    SAME_TYPE(false_, and_c<true, true, false>);
  }                                                         // mp_logical_and


  BOOST_AUTO_TEST_CASE(mp_logical_or)
  {
    SAME_TYPE(true_, or_<true_, true_>);
    SAME_TYPE(true_, or_<true_, true_, true_>);
    SAME_TYPE(true_, or_<false_, true_>);
    SAME_TYPE(true_, or_<true_, false_>);
    SAME_TYPE(true_, or_<false_, false_, false_, true_>);
    SAME_TYPE(false_, or_<false_, false_>);
    SAME_TYPE(false_, or_<false_, false_, false_>);
    // short circuit test
    SAME_TYPE(true_, or_<false_, true_, void_>);
    // or_c
    SAME_TYPE(false_, or_c<false, false>);
    SAME_TYPE(false_, or_c<false, false, false>);
    SAME_TYPE(true_, or_c<true, false>);
    SAME_TYPE(true_, or_c<false, true>);
    SAME_TYPE(true_, or_c<true, true>);
    SAME_TYPE(true_, or_c<false, false, true>);

  }                                                         // mp_logical_or

////////////////////////////////////////////////////////////////////////////////
//                            COMPARISON                                              
////////////////////////////////////////////////////////////////////////////////


#define EQUAL_TO(...)                    \
  CHECK( equal_to<__VA_ARGS__>() );      \
  CHECK( greater_equal<__VA_ARGS__>() ); \
  CHECK( less_equal<__VA_ARGS__>() );    \
  CHECK( !greater<__VA_ARGS__>() );      \
  CHECK( !less<__VA_ARGS__>() );         \
  CHECK( !not_equal_to<__VA_ARGS__>() )
#
#
#define NOT_EQUAL_TO(...)                                          \
  CHECK( not_equal_to<__VA_ARGS__>() );                            \
  CHECK( (less<__VA_ARGS__>() && less_equal<__VA_ARGS__>()         \
          && !greater_equal<__VA_ARGS__>())                        \
        || (greater<__VA_ARGS__>() && greater_equal<__VA_ARGS__>() \
            && !less_equal<__VA_ARGS__>())                         \
        );                                                         \
  CHECK( !equal_to<__VA_ARGS__>() )
#
#
#define LESS(...)                                            \
  CHECK( less<__VA_ARGS__>() && less_equal<__VA_ARGS__>() ); \
  CHECK( not_equal_to<__VA_ARGS__>() && !equal_to<__VA_ARGS__>() ); \
  CHECK( !greater<__VA_ARGS__>() && !greater_equal<__VA_ARGS__>() )
#
#
#define LESS_EQUAL(...)                                         \
  CHECK( less_equal<__VA_ARGS__>() );                           \
  CHECK( (equal_to<__VA_ARGS__>() && !less<__VA_ARGS__>())      \
        || (not_equal_to<__VA_ARGS__>() && less<__VA_ARGS__>()) \
        );                                                      \
  CHECK( !greater<__VA_ARGS__>() );                             \
  CHECK( equal_to<__VA_ARGS__>() == greater_equal<__VA_ARGS__>() )
#
#
#define GREATER(...)                                                \
  CHECK( greater<__VA_ARGS__>() && greater_equal<__VA_ARGS__>() );  \
  CHECK( not_equal_to<__VA_ARGS__>() && !equal_to<__VA_ARGS__>() ); \
  CHECK( !less<__VA_ARGS__>() && !less_equal<__VA_ARGS__>() )
#
#
#define GREATER_EQUAL(...)                                         \
  CHECK( greater_equal<__VA_ARGS__>() );                           \
  CHECK( (equal_to<__VA_ARGS__>() && !greater<__VA_ARGS__>())      \
        || (not_equal_to<__VA_ARGS__>() && greater<__VA_ARGS__>()) \
      );                                                           \
  CHECK( less_equal<__VA_ARGS__>() == equal_to<__VA_ARGS__>() );   \
  CHECK( !less<__VA_ARGS__>() )
#
  
  
 BOOST_AUTO_TEST_CASE(mp_comparision_test)
  {
    // LHS == RHS
    EQUAL_TO(true_, true_);
    EQUAL_TO(int_<1>, int_<1>);
    EQUAL_TO(char_<2>, long_<2>);
    EQUAL_TO(short_<10>, uint_<10>);
    // LHS != RHS
    NOT_EQUAL_TO(true_, false_);
    NOT_EQUAL_TO(short_<-10>, ullong_<10>);
    NOT_EQUAL_TO(int_<-1>, uint_<0>);
    // LHS < RHS
    LESS(false_, true_);
    LESS(int_<0>, int_<1>);
    LESS(short_<-1>, short_<1>);
    LESS(uint_<1>, uint_<10>);
    LESS(long_<0>, char_<2>);
    LESS(false_, ullong_<10>);
    // LHS <= RHS
    LESS_EQUAL(false_, false_);
    LESS_EQUAL(true_, true_);
    LESS_EQUAL(int_<-1>, int_<-1>);
    LESS_EQUAL(uint_<0>, int_<0>);
    // LHS > RHS
    GREATER(true_, false_);
    GREATER(int_<10>, char_<1>);
    GREATER(short_<100>, ullong_<99>);
    // LHS >= RHS
    GREATER_EQUAL(true_, true_);
    GREATER_EQUAL(false_, false_);
    GREATER_EQUAL(char_<10>, uint_<10>);
  }                                                  // mp_comparision_equality
  
BOOST_AUTO_TEST_SUITE_END()