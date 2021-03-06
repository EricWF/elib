#ifndef ELIB_EXCEPTION_ERROR_INFO_HPP
#define ELIB_EXCEPTION_ERROR_INFO_HPP

# include <elib/exception/fwd.hpp>
# include <elib/aux.hpp>

# define ELIB_EXCEPTION_ASSERT_ERROR_INFO_TYPE(Type)                      \
    static_assert(                                                     \
      ::elib::except::is_error_info< ::elib::aux::uncvref< Type>>::value \
      , #Type " is not an instance of error_info"                      \
    )

namespace elib { namespace except { inline namespace v1
{
    ////////////////////////////////////////////////////////////////////////////
    template <class Tag, class T> 
    class error_info
    {
    public:
        using tag_type = Tag;
        using value_type = T;
        
        error_info(T const & t)
          : m_value(t)
        {}
        
        error_info(T && t)
          : m_value(elib::move(t))
        {}
        
        T const & value() const noexcept { return m_value; }
        T & value() noexcept { return m_value; }

    private:
        T m_value;
    };
    
    template <class T>
    struct is_error_info : aux::false_ {};
    
    template <class Tag, class T>
    struct is_error_info< error_info<Tag, T> > : aux::true_ {};
    
}}}                                                          // namespace elib
#endif /* ELIB_EXCEPTION_ERROR_INFO_HPP */