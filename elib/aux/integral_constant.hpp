#ifndef ELIB_AUX_INTEGRAL_CONSTANT_HPP
#define ELIB_AUX_INTEGRAL_CONSTANT_HPP

# include <cstddef>

# /* type deduction macro for integral_constant */
# define ELIB_AUTO_INTC(...) \
    ::elib::integral_constant<decltype(__VA_ARGS__), __VA_ARGS__>
# 

namespace elib
{
    ////////////////////////////////////////////////////////////////////////////
    // integral_constant
    template <class T, T Val>
    struct integral_constant
    {
        using type = integral_constant;
        using value_type = T;
            
        static constexpr T value = Val;
            
        constexpr operator value_type() const noexcept
        { return Val; }
            
            constexpr T operator()() const noexcept
        { return Val; }
    };
    
    template <class T, T Val>
    constexpr T integral_constant<T, Val>::value;
    
    namespace aux
    {
        template <bool V>
        using bool_ = integral_constant<bool, V>;

        using true_ = bool_<true>;
        using false_ = bool_<false>;

        template <char V>
        using char_ = integral_constant<char, V>;
        
        template <short V>
        using short_ = integral_constant<short, V>;
        
        template <unsigned short V>
        using ushort_ = integral_constant<unsigned short, V>;

        template <int V>
        using int_ = integral_constant<int, V>;

        template <unsigned V>
        using uint_ = integral_constant<unsigned, V>;

        template <long V>
        using long_ = integral_constant<long, V>;

        template <unsigned long V>
        using ulong_ = integral_constant< unsigned long, V>;
        
        template <long long V>
        using llong_ = integral_constant<long long, V>;
        
        template <unsigned long long V>
        using ullong_ = integral_constant<unsigned long long, V>;
        
        template <std::size_t V>
        using size_type_ = integral_constant<std::size_t, V>;

        template <class To, class From>
        using intc_cast = 
            integral_constant<To, static_cast<To>(From::type::value)>;

        template <class From>
        using bool_cast = 
            integral_constant<bool, static_cast<bool>(From::type::value)>;
        
    }                                                       // namespace aux
    
    using aux::bool_;
    using aux::true_;
    using aux::false_;
    using aux::int_;
    using aux::size_type_;
}                                                           // namespace elib
#endif /* ELIB_AUX_INTEGRAL_CONSTANT_HPP */