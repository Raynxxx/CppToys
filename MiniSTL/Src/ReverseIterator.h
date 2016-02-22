/*
** ReverseIterator.h
** Created by Rayn on 2015/02/07
*/
#ifndef _REVERSE_ITERATOR_H_
#define _REVERSE_ITERATOR_H_

#include "Iterator.h"

namespace rayn {

    template<class Iterator>
    class reverse_iterator_t {
    public:
        typedef Iterator                                                iterator_type;
        typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
        typedef typename iterator_traits<Iterator>::value_type          value_type;
        typedef typename iterator_traits<Iterator>::difference_type     difference_type;
        typedef typename iterator_traits<Iterator>::pointer             pointer;
        typedef typename iterator_traits<Iterator>::reference           reference;
        typedef const pointer                                           const_pointer;
        typedef const reference                                         const_reference;


    protected:
        Iterator current;
        Iterator _cur;

    public:
        // constructor
        reverse_iterator_t() : current() {}

        explicit
        reverse_iterator_t(iterator_type it) : current(it) {}

        reverse_iterator_t(const reverse_iterator_t& it)
        : current(it.current) {}

        template <class Iter>
        reverse_iterator_t(const reverse_iterator_t<Iter>& it)
            : current(it.base()) {}


        // Access
        iterator_type
        base() const { return current; }

        reference
        operator*() const
        {
            Iterator tmp = current;
            return *--tmp;
        }

        pointer
        operator->()
        {
            return &(operator*());
        };


        // Modifier
        reverse_iterator_t&
        operator++()
        {
            --current;
            return *this;
        }

        reverse_iterator_t
        operator++(int)
        {
            reverse_iterator_t tmp = *this;
            --current;
            return tmp;
        }

        reverse_iterator_t&
        operator--()
        {
            ++current;
            return *this;
        }

        reverse_iterator_t
        operator--(int)
        {
            reverse_iterator_t tmp = *this;
            ++current;
            return tmp;
        }


        reverse_iterator_t
        operator+(difference_type n) const
        {
            return reverse_iterator_t(current - n);
        }

        reverse_iterator_t&
        operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }

        reverse_iterator_t
        operator-(difference_type n) const
        {
            return reverse_iterator_t(current + n);
        }

        reverse_iterator_t&
        operator-=(difference_type n)
        {
            current += n;
            return *this;
        }

        reference
        operator[] (difference_type n) const
        {
            return *(*this + n);
        }

    };


    template <class Iter>
    inline bool
    operator== (const reverse_iterator_t<Iter>& lhs,
                const reverse_iterator_t<Iter>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iter>
    inline bool
    operator!= (const reverse_iterator_t<Iter>& lhs,
                const reverse_iterator_t<Iter>& rhs)
    {
        return lhs.base() != rhs.base();
    }

    template <class Iter>
    inline bool
    operator< (const reverse_iterator_t<Iter>& lhs,
               const reverse_iterator_t<Iter>& rhs)
    {
        return lhs.base() > rhs.base();
    }

    template <class Iter>
    inline bool
    operator> (const reverse_iterator_t<Iter>& lhs,
               const reverse_iterator_t<Iter>& rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <class Iter>
    inline bool
    operator<= (const reverse_iterator_t<Iter>& lhs,
                const reverse_iterator_t<Iter>& rhs)
    {
        return !(lhs > rhs);
    }

    template <class Iter>
    inline bool
    operator>= (const reverse_iterator_t<Iter>& lhs,
                const reverse_iterator_t<Iter>& rhs)
    {
        return !(lhs < rhs);
    }

    template <class Iter>
    inline reverse_iterator_t<Iter>
    operator+ (typename reverse_iterator_t<Iter>::difference_type n,
               const reverse_iterator_t<Iter>& it)
    {
        return reverse_iterator_t<Iter>(it + n);
    }

    template <class Iter>
    inline typename reverse_iterator_t<Iter>::difference_type
    operator- (const reverse_iterator_t<Iter>& lhs,
               const reverse_iterator_t<Iter>& rhs)
    {
        return lhs.base() - rhs.base();
    }
}
#endif