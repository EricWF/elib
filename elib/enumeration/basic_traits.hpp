#ifndef ELIB_ENUMERATION_BASIC_TRAITS_HPP
#define ELIB_ENUMERATION_BASIC_TRAITS_HPP

# include <elib/aux/integral_constant.hpp>
# include <string>
# include <map>

namespace elib { namespace enumeration { inline namespace v1
{
    
# if defined(ELIB_ENUMERATION_EXPOSITION)
#   error ELIB_ENUMERATION_EXPOSITION should never be defined
    template <typename T>
    struct basic_enum_traits
    {
        static const std::map<T, std::string> name_map;
        
        static constexpr T ELIB_ENUM_DEFAULT_VALUE = T::value; 
        static constexpr T ELIB_ENUM_ERROR_VALUE = T::value;
        
        static constexpr T ELIB_ENUM_FIRST_VALUE = T::value;
        static constexpr T ELIB_ENUM_LAST_VALUE = T::value;
        
        static constexpr bool ELIB_ENUM_IS_CONTIGIOUS = false;
    };
#endif

    /* Actual implementation of basic_enum_traits, 
     * it has no members, except one used to detect if it is the default
     * instantiation
     */
    template <class T>
    struct basic_enum_traits
    {
        static constexpr bool ELIB_ENUM_IS_DEFAULT_ENUM_TRAITS = true;
    };
      
    namespace detail
    {
     
        ////////////////////////////////////////////////////////////////////////
        template <
            class T
          , template <class Test> class ApplyTest
        >
        struct basic_traits_detector_impl
        {
        private:
            template <class U>
            static aux::true_ test(ApplyTest<U>*);
            
            template <class>
            static aux::false_ test(...);
        public:
            using type = decltype(test<T>(0));
        };
      
        ////////////////////////////////////////////////////////////////////////
        template <
            class T
          , template <class Test> class ApplyTest 
        >
        struct basic_traits_detector 
          : basic_traits_detector_impl<T, ApplyTest>::type
        {};
     
        ////////////////////////////////////////////////////////////////////////
        template <class T>
        using basic_traits_is_default_detector = 
            decltype(basic_enum_traits<T>::ELIB_ENUM_IS_DEFAULT_ENUM_TRAITS);
        
        ////////////////////////////////////////////////////////////////////////
        template <class T>
        using basic_traits_name_map_detector = 
            decltype(basic_enum_traits<T>::name_map);
    }                                                       // namespace detail
    
    ////////////////////////////////////////////////////////////////////////////
    template <class T>
    struct has_basic_enum_traits 
      : bool_< 
          !detail::basic_traits_detector<
            T, detail::basic_traits_is_default_detector
          >::value
        >
    {};
    
    ////////////////////////////////////////////////////////////////////////////
    template <class T, bool=has_basic_enum_traits<T>::value>
    struct has_name_map : aux::false_ {};
    
    template <class T>
    struct has_name_map<T, true>
      : aux::bool_<
          detail::basic_traits_detector<
            T, detail::basic_traits_name_map_detector
          >::value
        >
    {};
}}}                                                          // namespace elib
#endif /* ELIB_ENUMERATION_BASIC_TRAITS_HPP */
