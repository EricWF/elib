#ifndef ELIB_ENUMERATION_OPERATORS_HPP
#define ELIB_ENUMERATION_OPERATORS_HPP

# include <elib/enumeration/cast.hpp>
# include <elib/aux/auto_return.hpp>
# include <elib/preprocessor/cat.hpp>

# define ELIB_ENUMERATION_PREFIX_OP(Type, Op)                       \
    inline Type & operator ELIB_PP_CAT(Op, Op) (Type & t)           \
    ELIB_RETURN_NOEXCEPT(t = static_cast<Type>(                     \
        static_cast< ::elib::enumeration::opt_cast_t<Type>>(t) Op 1 \
    ))
# 

# define ELIB_ENUMERATION_SUFFIX_OP(Type, Op)                \
    inline Type operator ELIB_PP_CAT(Op, Op) (Type & t, int) \
    {                                                        \
        auto cp = t;                                         \
        ELIB_PP_CAT(Op, Op) t;                               \
        return cp;                                           \
    } 
# 
        
# define ELIB_ENUMERATION_UNARY_OP(Ret, Type, Op)                 \
    constexpr Ret operator Op (Type t)                            \
    ELIB_RETURN_NOEXCEPT(static_cast<Ret>(                        \
        Op static_cast< ::elib::enumeration::opt_cast_t<Type>>(t) \
    ))
    
# define ELIB_ENUMERATION_BINARY_OP(Ret, Type0, Type1, Op)       \
    constexpr Ret operator Op (Type0 t0, Type1 t1)               \
    ELIB_RETURN_NOEXCEPT(static_cast<Ret>(                       \
        static_cast< ::elib::enumeration::opt_cast_t<Type0>>(t0) \
        Op                                                       \
        static_cast< ::elib::enumeration::opt_cast_t<Type1>>(t1) \
    ))
# 

# define ELIB_ENUMERATION_COMPOUND_OP(Type0, Type1, Op)        \
    Type0 & operator ELIB_PP_CAT(Op, =) (Type0 & t0, Type1 t1) \
    ELIB_RETURN_NOEXCEPT(t0 = (t0 Op t1))
# 
    
#endif /* ELIB_ENUMERATION_OPERATORS_HPP */
