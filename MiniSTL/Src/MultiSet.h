/*
** MultiSet.h
** Created by Rayn on 2016/02/21
*/
#ifndef _MULTISET_H_
#define _MULTISET_H_

#include "Tree.h"
#include "Functional.h"

namespace rayn {

    template <class T, class Compare = rayn::less<T>>
    class multiset {
    public:
        // public typedefs
        typedef T           key_type;
        typedef T           value_type;
        typedef Compare     key_compare;
        typedef Compare     value_compare;

    private:
        typedef rb_tree<key_type, value_type, identity<value_type>,
            key_compare>    _rep_type;

        _rep_type   _m_tree;

    public:
        // public typedefs of iterator.
        typedef typename _rep_type::const_pointer           pointer;
        typedef typename _rep_type::const_pointer           const_pointer;
        typedef typename _rep_type::const_reference         reference;
        typedef typename _rep_type::const_reference         const_reference;
        typedef typename _rep_type::const_iterator          iterator;
        typedef typename _rep_type::const_iterator          const_iterator;
        typedef typename _rep_type::const_reverse_iterator  reverse_iterator;
        typedef typename _rep_type::const_reverse_iterator  const_reverse_iterator;
        typedef typename _rep_type::size_type               size_type;
        typedef typename _rep_type::difference_type         difference_type;

        // constructor/destructor
        multiset() : _m_tree() {}

        explicit
        multiset(const Compare& comp) : _m_tree(comp) {}

        template <typename InputIterator>
        multiset(InputIterator first, InputIterator last) : _m_tree()
        {
            _m_tree.insert_equal(first, last);
        }

        template <typename InputIterator>
        multiset(InputIterator first, InputIterator last, const Compare& comp)
        : _m_tree(comp)
        {
            _m_tree.insert_equal(first, last);
        }

        multiset(const multiset& s) : _m_tree(s._m_tree) {}

        multiset(multiset&& s) : _m_tree(rayn::move(s._m_tree)) {}

        multiset& operator=(const multiset& s) {
            _m_tree = s._m_tree;
            return *this;
        }

        // Iterators
        iterator
        begin()
        {
            return _m_tree.begin();
        }

        const_iterator
        begin() const
        {
            return _m_tree.begin();
        }

        iterator
        end()
        {
            return _m_tree.end();
        }

        const_iterator
        end() const
        {
            return _m_tree.end();
        }

        reverse_iterator
        rbegin()
        {
            return _m_tree.rbegin();
        }

        const_reverse_iterator
        rbegin() const
        {
            return _m_tree.rbegin();
        }

        reverse_iterator
        rend()
        {
            return _m_tree.rend();
        }

        const_reverse_iterator
        rend() const
        {
            return _m_tree.rend();
        }

        const_iterator
        cbegin() const
        {
            return _m_tree.begin();
        }

        const_iterator
        cend() const
        {
            return _m_tree.end();
        }

        const_reverse_iterator
        crbegin() const
        {
            return _m_tree.rbegin();
        }

        const_reverse_iterator
        crend() const
        {
            return _m_tree.rend();
        }

        // Capacity
        bool        empty() const       { return _m_tree.empty(); }
        size_type   size() const        { return _m_tree.size(); }
        size_type   max_size() const    { return _m_tree.max_size(); }

        // Modifiers
        pair<iterator, bool>
        insert(const value_type& val)
        {
            pair<typename _rep_type::iterator, bool> ret =
                _m_tree.insert_equal(val);
            return pair<iterator, bool>(ret.first, ret.second);
        }

        template <class InputIterator>
        void
        insert(InputIterator first, InputIterator last)
        {
            _m_tree.insert_equal(first, last);
        }

        iterator
        erase(const_iterator pos)
        {
            return _m_tree.erase(pos);
        }

        size_type
        erase(const key_type& k)
        {
            return _m_tree.erase(k);
        }

        iterator
        erase(const_iterator first, const_iterator last)
        {
            return _m_tree.erase(first, last);
        }

        void
        swap(multiset& x) { _m_tree.swap(x._m_tree); }

        void
        clear() { _m_tree.clear(); }

        // Observers
        key_compare     key_comp() const    { return _m_tree.key_comp(); }
        value_compare   value_comp() const  { return _m_tree.key_comp(); }

        // Operations
        iterator
        find(const key_type& k) { return _m_tree.find(k); }

        const_iterator
        find(const key_type& k) const { return _m_tree.find(k); }

        size_type
        count(const key_type& k) const
        {
            return _m_tree.count(k);
        }

        iterator
        lower_bound(const key_type& k)
        {
            return _m_tree.lower_bound(k);
        }

        const_iterator
        lower_bound(const key_type& k) const
        {
            return _m_tree.lower_bound(k);
        }

        iterator
        upper_bound(const key_type& k)
        {
            return _m_tree.upper_bound(k);
        }

        const_iterator
        upper_bound(const key_type& k) const
        {
            return _m_tree.upper_bound(k);
        }

        pair<iterator, iterator>
        equal_range(const key_type& k)
        {
            return _m_tree.equal_range(k);
        }

        pair<const_iterator, const_iterator>
        equal_range(const key_type& k) const
        {
            return _m_tree.equal_range(k);
        }

        // friend functions
        template <class T2, class Compare2>
        friend bool
        operator==(const multiset<T2, Compare2>&, const multiset<T2, Compare2>&);

        template <class T2, class Compare2>
        friend bool
        operator<(const multiset<T2, Compare2>&, const multiset<T2, Compare2>&);
    };

    template <class T, class Compare>
    inline bool
    operator==(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        return x._m_tree == y._m_tree;
    }

    template <class T, class Compare>
    inline bool
    operator!=(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        return !(x == y);
    }

    template <class T, class Compare>
    inline bool
    operator<(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        return x._m_tree < y._m_tree;
    }

    template <class T, class Compare>
    inline bool
    operator>(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        return y < x;
    }

    template <class T, class Compare>
    inline bool
    operator<=(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        return !(y < x);
    }

    template <class T, class Compare>
    inline bool
    operator>=(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        return !(x < y);
    }

    template <class T, class Compare>
    inline void
    swap(const multiset<T, Compare>& x, const multiset<T, Compare>& y)
    {
        x.swap(y);
    }

}

#endif