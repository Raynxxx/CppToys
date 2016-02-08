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
        typedef Iterator    iterator_type;
        typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
        typedef typename iterator_traits<Iterator>::value_type          value_type;
        typedef typename iterator_traits<Iterator>::difference_type     difference_type;
        typedef typename iterator_traits<Iterator>::pointer             pointer;
        typedef typename iterator_traits<Iterator>::reference           reference;
        typedef const pointer       const_pointer;
        typedef const reference     const_reference;


    protected:
        Iterator _base;
        Iterator _cur;

    public:
        //The default constructor
        reverse_iterator_t() : _base(0), _cur(0) {}
        //The explicit constructor with normal iterator, then will move in the opposite direction.
        explicit reverse_iterator_t(const iterator_type& it) : _base(it) {
            auto temp = it;
            this->_cur = --temp;
        }
        //The copy constructor
        template <class Iter>
        reverse_iterator_t(const reverse_iterator_t<Iter>& rev_it) {
            _base = (iterator_type) rev_it.base();
            auto temp = _base;
            _cur = --temp;
        }

        /*
        ** @return @c _base, the iterator used for underlying work.
        */
        iterator_type base() {
            return _base;
        }
        /*
        ** @return A reference to the value at @c _cur.
        ** This requires that @c _cur is dereferenceable.
        */
        reference operator*() { return *_cur; }
        const_reference operator*() const { return *_cur; }
        /*
        ** @return A pointer to the value at @c _cur;
        */
        pointer operator->() { return &(operator*()) };
        const_pointer operator->() const { return &(operator*()); }

        /*
        ** @return @c *this after decrement.
        ** Decrements the underlying iterator.
        */
        reverse_iterator_t& operator++() {
            --_base;
            --_cur;
            return *this;
        }
        /*
        ** @return @c *this The orginal value.
        ** Decrements the underlying iterator.
        */
        reverse_iterator_t operator++(int) {
            reverse_iterator tmp = *this;
            --_base;
            --_cur;
            return tmp;
        }
        /*
        ** @return @c *this after increment.
        ** Increments the underlying iterator.
        */
        reverse_iterator_t& operator--() {
            ++_base;
            ++_cur;
            return *this;
        }
        /*
        ** @return @c *this The orginal value.
        ** Increments the underlying iterator.
        */
        reverse_iterator_t operator--(int) {
            reverse_iterator tmp = *this;
            ++_base;
            ++_cur;
            return tmp;
        }

        /*
        ** @return  A reverse_iterator that refers to @c _base - n
        ** The underlying iterator must be a Random Access Iterator.
        */
        reverse_iterator_t operator+(difference_type n) const {
            return reverse_iterator_t(_base - n);
        }
        /*
        ** @return *this after increase n pos.
        ** @brief Moves the underlying iterator backwards @c n steps.
        ** The underlying iterator must be a Random Access Iterator.
        */
        reverse_iterator_t& operator+=(difference_type n) {
            _base -= n;
            _cur -= n;
            return *this;
        }
        /*
        ** @return  A reverse_iterator that refers to @c _base + n
        ** The underlying iterator must be a Random Access Iterator.
        */
        reverse_iterator_t operator-(difference_type n) const {
            return reverse_iterator_t(_base + n);
        }
        /*
        ** @return *this
        ** @brief Moves the underlying iterator forwards @c n steps.
        ** The underlying iterator must be a Random Access Iterator.
        */
        reverse_iterator_t& operator-=(difference_type n) {
            _base += n;
            _cur += n;
            return *this;
        }

        /*
        ** @return The value at @c _cur - n.
        */
        const_reference operator[] (difference_type n) const {
            return *(*this + n);
        }
    
        /*
        ** @param lhs A reverse_iterator
        ** @param rhs A reverse_iterator
        ** @return A boolean value
        ** @brief forward many operations to their underlying base() iterators.
        */
        template <class Iter>
        friend bool operator == (const reverse_iterator_t<Iter>& lhs, const reverse_iterator_t<Iter>& rhs) {
            return lhs.base() == rhs.base();
        }
        template <class Iter>
        friend bool operator != (const reverse_iterator_t<Iter>& lhs, const reverse_iterator_t<Iter>& rhs) {
            return lhs.base() != rhs.base();
        }
        template <class Iter>
        friend bool operator < (const reverse_iterator_t<Iter>& lhs, const reverse_iterator_t<Iter>& rhs) {
            return lhs.base() > rhs.base();
        }
        template <class Iter>
        friend bool operator > (const reverse_iterator_t<Iter>& lhs, const reverse_iterator_t<Iter>& rhs) {
            return lhs.base() < rhs.base();
        }
        template <class Iter>
        friend bool operator <= (const reverse_iterator_t<Iter>& lhs, const reverse_iterator_t<Iter>& rhs) {
            return !(lhs > rhs);
        }
        template <class Iter>
        friend bool operator >= (const reverse_iterator_t<Iter>& lhs, const reverse_iterator_t<Iter>& rhs) {
            return !(lhs < rhs);
        }
        
    };

    template <class Iter>
    reverse_iterator_t<Iter> operator+ (typename reverse_iterator_t<Iter>::difference_type n,
        const reverse_iterator_t<Iter>& rev_it) {
        return rev_it + n;
    }

    template <class Iter>
    typename reverse_iterator_t<Iter>::difference_type operator- (const reverse_iterator_t<Iter>& lhs,
        const reverse_iterator_t<Iter>& rhs) {
        return lhs.base() - rhs.base();
    }
}
#endif