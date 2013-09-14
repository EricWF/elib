#ifndef ELIB_ENUMERATION_ENUM_CAST_H
#define ELIB_ENUMERATION_ENUM_CAST_H

#include <elib/enumeration/basic_enum_traits.h>
#include <elib/enumeration/enum.h>

#include <stdexcept>
#include <string>
#include <type_traits>

namespace elib {
namespace enumeration {
    
    
/* the exception thrown when any of the non-safe casts fail */
class bad_enum_cast : public std::logic_error {
public:
    bad_enum_cast()
        : std::logic_error("bad enum cast")
    { }
    
    bad_enum_cast(const std::string & what)
        : std::logic_error("bad enum cast: " + what)
    { }
};


/* cast strings and underlying_types to Enum values */
template <typename EnumT>
inline enable_if_has_traits_t<EnumT>
enum_cast(base_t<EnumT> base)
{
    EnumT tmp = static_cast<EnumT>(base);
    if (! basic_enum_traits<EnumT>::name_map.count(tmp))
        throw bad_enum_cast();
    return tmp;
}

template <typename EnumT>
inline enable_if_has_traits_t<EnumT>
enum_cast(const std::string& str)
{
    typedef basic_enum_traits<EnumT> traits;
    for (auto& kv : traits::name_map) {
        if (kv.second == str)
            return kv.first;
    }
    throw bad_enum_cast(str);
}

/* these versions are nothrow
 * on success they return true, and write to dest,
 * otherwise dest is ignored and false is return */
template <typename EnumT>
inline enable_if_has_traits_t<EnumT, bool>
enum_cast(base_t<EnumT> base, EnumT& dest) noexcept
{
    EnumT tmp = static_cast<EnumT>(base);
    if (basic_enum_traits<EnumT>::name_map.count(tmp)) {
        dest = tmp;
        return true;
    }
    
    dest = npos_enum<EnumT>();
    return false;
}

template <typename EnumT>
inline enable_if_has_traits_t<EnumT, bool>
enum_cast(const std::string& str, EnumT& dest) noexcept
{
    for (auto& kv : basic_enum_traits<EnumT>::name_map) {
        if (kv.second == str) {
            dest = kv.first;
            return true;
        }
    }
    
    dest = npos_enum<EnumT>();
    return false;
}

/* since enum cast is overloaded, use this to access enum_cast 
 * via non-overloaded methods */
template <typename EnumT>
inline EnumT
enum_cast_string(const std::string& s)
{
    return enum_cast<EnumT>(s);
}

template <typename EnumT>
inline EnumT
enum_cast_base(base_t<EnumT> base)
{
    return enum_cast<EnumT>(base);
}

namespace detail {
    
    template <typename EnumT, typename Ret>
    struct lexical_cast_helper {};

    template <typename EnumT>
    struct lexical_cast_helper<EnumT, base_t<EnumT>> {

        static inline base_t<EnumT>
        cast(EnumT e)
        {
            if (basic_enum_traits<EnumT>::name_map.count(e))
                return static_cast<base_t<EnumT>>(e);
            throw bad_enum_cast();
        }
    };

    template <typename EnumT>
    struct lexical_cast_helper<EnumT, std::string> {
    
        static inline std::string
        cast(EnumT e)
        {
            if (basic_enum_traits<EnumT>::name_map.count(e))
                return basic_enum_traits<EnumT>::name_map.at(e);
            throw bad_enum_cast();
        }
    };
    
} // namespace detail 


/* Cast Enum values to std::string or the underlying_type */
template <typename Ret, typename EnumT>
inline enable_if_has_traits_t<EnumT, Ret>
lexical_enum_cast(EnumT e)
{
    return detail::lexical_cast_helper<EnumT, Ret>::cast(e);
}


    
} /* namespace enumeration */
} /* namespace elib */
#endif /* ELIB_ENUMERATION_ENUM_CAST_H */