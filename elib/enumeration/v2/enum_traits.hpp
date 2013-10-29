#ifndef ELIB_ENUMERATION_ENUM_TRAITS_HPP
#define ELIB_ENUMERATION_ENUM_TRAITS_HPP


# include <elib/config.hpp>
# include <elib/enumeration/v2/basic_enum_traits.hpp>
# include <elib/enumeration/v2/enum_fields.hpp>
# include <elib/enumeration/v2/detail/enum_type_traits_helper.hpp>

# include <elib/CXX14/type_traits.hpp>

# include <iterator>
# include <cstddef>


namespace elib 
{
  namespace enumeration
  {
    
    template <typename T, typename Ret=void>
    struct enable_if_enum : public std::enable_if<std::is_enum<T>::value, Ret>
    { };
    
    template <typename T, typename Ret=void>
    using enable_if_enum_t = typename enable_if_enum<T, Ret>::type;
    
    template <typename T>
    struct has_basic_enum_traits
    {
    private:
      template <typename U>
      static std::false_type test(typename basic_enum_traits<U>::m_default*);

      template <typename U>
      static std::true_type test(...);

    public:
      typedef decltype(test<T>(0)) type;
      typedef typename type::value_type value_type;
      static constexpr bool value = type::value;
    };
  
  
    template <typename T>
    struct has_constexpr_bounds 
      : public std::integral_constant<bool, first_field<T>::good 
            && last_field<T>::good>
    { };

    
    template <typename T>
    struct has_bounds 
      : public std::integral_constant<bool, has_constexpr_bounds<T>::value 
            || has_basic_enum_traits<T>::value>
    { };

    
    template <typename T>
    struct has_constexpr_range
      : public std::integral_constant<bool, has_constexpr_bounds<T>::value 
          && is_contiguous_field<T>::bool_value>
    { };

    
    template <typename T>
    struct has_range
      : public std::integral_constant<bool, has_constexpr_range<T>::value ||
        has_basic_enum_traits<T>::value>
    { };
    
    
    namespace detail
    {
      
      
      template <typename EnumT, typename=void>
      struct enum_bounds;
      
      
      template <typename EnumT>
      struct enum_bounds<EnumT, 
          std::enable_if_t<has_constexpr_bounds<EnumT>::value>>
      {
        static constexpr EnumT min = first_field<EnumT>::value;
        static constexpr EnumT max = last_field<EnumT>::value; 
      };
      
      
      template <typename EnumT>
      struct enum_bounds<EnumT, std::enable_if_t<has_bounds<EnumT>::value && 
            !has_constexpr_bounds<EnumT>::value>>
      {
        
        static const EnumT min;
        static const EnumT max;
      };                                                    // struct enum_bounds
      
      
      template <typename EnumT>
      const EnumT 
      enum_bounds<EnumT, std::enable_if_t<has_bounds<EnumT>::value && 
          !has_constexpr_bounds<EnumT>::value>>::min = 
            basic_enum_traits<EnumT>::name_map.cbegin()->first;
            
            
      template <typename EnumT>
      const EnumT 
      enum_bounds<EnumT, std::enable_if_t<has_bounds<EnumT>::value && 
        !has_constexpr_bounds<EnumT>::value>>::max = 
          basic_enum_traits<EnumT>::name_map.crbegin()->first;

        
    }                                                       // namespace detail 
    
    
    template <typename EnumT>
    struct enum_bounds : public detail::enum_bounds<EnumT>
    { };

    
    template <typename EnumT>
    constexpr std::size_t size_of_bounds(EnumT min, EnumT max) noexcept
    {
      using utype = std::underlying_type_t<EnumT>;
      return static_cast<std::size_t>(static_cast<utype>(max) - 
        static_cast<utype>(min) + 1);
    }

    
    template <typename EnumT>
    constexpr std::enable_if_t<has_constexpr_bounds<EnumT>::value, std::size_t>
    size_of_bounds() noexcept
    {
      using bounds = enum_bounds<EnumT>;
      return size_of_bounds(bounds::min, bounds::max);
    }
    
    
    template <typename EnumT>
    std::enable_if_t<has_bounds<EnumT>::value 
        && !has_constexpr_bounds<EnumT>::value,  std::size_t>
    size_of_bounds() noexcept
    {
      using bounds = enum_bounds<EnumT>;
      return size_of_bounds(bounds::min, bounds::max);
    }
    
   
    
    namespace detail
    {
      
      
      template <typename EnumT, typename=void>
      struct enum_range;
      
      
      template <typename EnumT>
      struct enum_range<EnumT, 
          std::enable_if_t<has_constexpr_range<EnumT>::value>>
        : public enum_bounds<EnumT>
      {
        static constexpr std::size_t size = 
          size_of_bounds(enum_bounds<EnumT>::min, 
                         enum_bounds<EnumT>::max);
      };
      
      
      template <typename EnumT>
      struct enum_range<EnumT, std::enable_if_t<has_range<EnumT>::value && 
            !has_constexpr_range<EnumT>::value>>
        : public enum_bounds<EnumT>  
      {
        static const std::size_t size;
      };
      
      
      template <typename EnumT>
      const std::size_t 
      enum_range<EnumT, std::enable_if_t<has_range<EnumT>::value && 
        !has_constexpr_range<EnumT>::value>>::size = size_of_bounds<EnumT>();
      
      
    }                                                       // namespace detail
    
   
    template <typename T>
    struct enum_range : public detail::enum_range<T>
    { };
    
    
    template <typename T>
    constexpr std::enable_if_t<has_constexpr_range<T>::value, bool>
    valid_enum(T val) noexcept
    {
      // ensure constexpr-ness of function
      static_assert(enum_range<T>::size == enum_range<T>::size, "");
      
      return (enum_range<T>::size != 0 
              && val >= enum_range<T>::min
              && val <= enum_range<T>::max);
    }
    
    template <typename T>
    std::enable_if_t<has_range<T>::value 
        && !has_constexpr_range<T>::value
        && is_contiguous_field<T>::bool_value, bool>
    valid_enum(T val) noexcept
    {
      return (enum_range<T>::size != 0
              && val >= enum_range<T>::min 
              && val <= enum_range<T>::value);
    }
    
    /* has_range && !has_constexpr_range -> has_basic_enum_traits */
    template <typename T>
    std::enable_if_t<has_range<T>::value 
        && !has_constexpr_range<T>::value
        && !is_contiguous_field<T>::bool_value, bool>
    valid_enum(T val)
    {
      return basic_enum_traits<T>::name_map.count(val) != 0;
    }
    
  }                                                    // namespace enumeration
}                                                           // namespace elib
#endif /* ELIB_ENUMERATION_ENUM_TRAITS_HPP */