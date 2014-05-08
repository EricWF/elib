#ifndef ELIB_ENUMERATION_ITERATOR_HPP
#define ELIB_ENUMERATION_ITERATOR_HPP

# include <elib/enumeration/basic_traits.hpp>
# include <elib/enumeration/traits.hpp>
# include <elib/aux.hpp>

# include <iterator>

namespace elib { namespace enumeration
{
    
    namespace detail
    {
        template <class T>
        struct is_iterable : has_range<T> {};
        
        template <class T, bool=aux::is_enum<T>::value>
        struct is_constexpr_range_iterable : aux::false_
        {};
        
        template <class T>
        struct is_constexpr_range_iterable<T, true>
            : bool_<enum_traits<T>::has_constexpr_range>
        {};
        
        template <class T>
        struct is_name_map_iterable : has_name_map<T>
        {};
        
        template <class T>
        using enable_if_constexpr_iterable_t =
            aux::enable_if_t<is_constexpr_range_iterable<T>>;
            
        template <class T>
        using enable_if_name_map_iterable_t =
            aux::enable_if_c_t<
            is_name_map_iterable<T>::value
                && !is_constexpr_range_iterable<T>::value
            >;
            
        template <class T>
        using enable_if_iterable_t =
            aux::enable_if_t<is_iterable<T>>;
        
        
        template <class T, class=void>
        class iter_impl;
        
        
        template <class T>
        class iter_impl<T, enable_if_constexpr_iterable_t<T>>
            : public std::iterator<std::bidirectional_iterator_tag, T>
        {
        private:
            static constexpr T m_end_value = 
            static_cast<T>(
                underlying_cast(enum_traits<T>::last_value) + 1
            );
            
        public:
            
            iter_impl()
            : m_val{m_end_value}
            {}
            
            explicit iter_impl(T val)
            : m_val{val}
            {}
            
            virtual ~iter_impl() noexcept {}
            
        protected:
            
            const T& dereference() const
            { return m_val; }
            
            bool equals(const iter_impl& other) const
            {
            return (m_val == other.m_val);
            }
            
            void increment()
            {
            if (m_val != m_end_value)
                m_val = static_cast<T>(underlying_cast(m_val) + 1);
            }
            
            void decrement()
            {
            if (m_val != first_value<T>())
                m_val = static_cast<T>(underlying_cast(m_val) - 1);
            }

        private:
            
            T m_val {};
            
        };
        
        
        template <class T>
        class iter_impl<T, enable_if_name_map_iterable_t<T>>
            : public std::iterator<std::bidirectional_iterator_tag, T>
        {
        private:
            
            static_assert(has_name_map<T>::value, "must have name map");
            
            using btraits = basic_enum_traits<T>;
            
            using result = decltype(btraits::name_map);
            using map_iterator = typename result::const_iterator;
            
        
        public:
            
            iter_impl()
            : m_iter{btraits::name_map.end()}
            {}
            
            explicit iter_impl(T pos)
            : m_iter{btraits::name_map.find(pos)}
            {}
            
            virtual ~iter_impl() noexcept {}
            
        protected:
            
            const T& dereference() const
            { return m_iter->first; }
            
            bool equals(const iter_impl& other) const
            { return m_iter == other.m_iter; }
            
            void increment()
            { ++m_iter; }
            
            void decrement()
            { --m_iter; }
            
        private:
            
            map_iterator m_iter {};
            
        };                                           // class iter_impl(name_map)
        
    }                                                       // namespace detail
    
    
    template <typename T, class=detail::enable_if_iterable_t<T>>
    class enum_iterator 
      : private detail::iter_impl<T>
    {
        using base_type = detail::iter_impl<T>;
    public:
        
        enum_iterator()
          : base_type()
        {}
        
        explicit enum_iterator(T v)
          : base_type(v)
        {}
        
        const T& operator*() const
        {
            return this->dereference();
        }
        
        bool operator==(const enum_iterator& other) const
        { return this->equals(other); }
        
        bool operator!=(const enum_iterator& other) const
        { return ! this->equals(other); }
        
        enum_iterator& operator++()
        {
            this->increment();
            return *this;
        }
        
        enum_iterator operator++(int)
        {
            auto cp = *this;
            this->increment();
            return cp;
        }
        
        enum_iterator& operator--()
        {
            this->decrement();
            return *this;
        }
        
        enum_iterator operator--(int)
        {
            auto cp = *this;
            this->decrement();
            return cp;
        }
    };                                                   // class enum_iterator
    
}}                                                          // namespace elib
#endif /* ELIB_ENUMERATION_ITERATOR_HPP */