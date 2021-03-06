#ifndef ELIB_ITERATOR_FILTER_ITERATOR_HPP
#define ELIB_ITERATOR_FILTER_ITERATOR_HPP

# include <elib/aux/move.hpp>
# include <iterator>

namespace elib { namespace iter { inline namespace v1
{
    ////////////////////////////////////////////////////////////////////////////
    template<class Iterator, class Pred>
    class filter_iterator
    {
    private:
        using self = filter_iterator;
        using Traits = std::iterator_traits<Iterator>;
    public:
        using value_type = typename Traits::value_type;
        using reference = typename Traits::reference;
        using pointer = typename Traits::pointer;
        using difference_type = typename Traits::difference_type;
        using iterator_category = std::forward_iterator_tag;
        
    public:
    
        filter_iterator(Pred p, Iterator b, Iterator e = Iterator())
          : m_pred(elib::move(p)), m_pos(b), m_end(e)
        {
            satify_pred();
        }
        
        filter_iterator(filter_iterator const &) = default;
        filter_iterator(filter_iterator &&) = default;
        filter_iterator & operator=(filter_iterator const &) = default;
        filter_iterator & operator=(filter_iterator &&) = default;
        
        bool operator==(self const & other) const { return m_pos == other.m_pos; }
        bool operator!=(self const & other) const { return m_pos != other.m_pos; }
        
        reference operator*()  const { return *m_pos; }
        pointer   operator->() const { return m_pos.operator->(); }
        
        self & operator++() { increment(); return *this; }
        
        filter_iterator operator++(int)
        {
            auto cp = *this;
            increment();
            return cp;
        }
        
        Iterator position() { return m_pos; }
        
    private:
        void increment() { ++m_pos; satify_pred(); }
        
        void satify_pred() 
        { 
            while (m_pos != m_end && !m_pred(*m_pos))
                ++m_pos; 
        }
        
    private:
        Pred m_pred;
        Iterator m_pos;
        Iterator m_end;
    };
}}}                                                          // namespace elib
#endif /* ELIB_ITERATOR_FILTER_ITERATOR_HPP */