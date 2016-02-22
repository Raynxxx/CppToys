/*
** Set.h
** Created by Rayn on 2016/02/21
*/
#ifndef _MULTIMAP_H_
#define _MULTIMAP_H_

#include "Tree.h"
#include "Functional.h"

namespace rayn {

    template <class Key, class T, class Compare = rayn::less<Key>>
    class multimap {
    public:
        // public typedefs
        typedef Key                 key_type;
        typedef T                   mapped_type;
        typedef pair<const Key, T>  value_type;
        typedef Compare             key_compare;

        class value_compare
        : public binary_function < value_type, value_type, bool >
        {
            friend class multimap<Key, T, Compare>;

        protected:
            Compare comp;

            value_compare(Compare c) : comp(c) {}

        public:
            bool operator()(const value_type& x, const value_type& y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        typedef rb_tree<key_type, value_type, select1st<value_type>,
                        key_compare>    _rep_type;

        _rep_type   _m_tree;

    public:
        // public typedefs of iterator.
        typedef typename _rep_type::pointer                 pointer;
        typedef typename _rep_type::const_pointer           const_pointer;
        typedef typename _rep_type::reference               reference;
        typedef typename _rep_type::const_reference         const_reference;
        typedef typename _rep_type::iterator                iterator;
        typedef typename _rep_type::const_iterator          const_iterator;
        typedef typename _rep_type::reverse_iterator        reverse_iterator;
        typedef typename _rep_type::const_reverse_iterator  const_reverse_iterator;
        typedef typename _rep_type::size_type               size_type;
        typedef typename _rep_type::difference_type         difference_type;

        // constructor/destructor
        multimap() : _m_tree() {}

        explicit
        multimap(const Compare& comp) : _m_tree(comp) {}

        template <typename InputIterator>
        multimap(InputIterator first, InputIterator last) : _m_tree()
        {
            _m_tree.insert_equal(first, last);
        }

        template <typename InputIterator>
        multimap(InputIterator first, InputIterator last, const Compare& comp)
        : _m_tree(comp)
        {
            _m_tree.insert_equal(first, last);
        }

        multimap(const multimap& m) : _m_tree(m._m_tree) {}

        multimap(multimap&& m) : _m_tree(rayn::move(m._m_tree)) {}

        multimap& operator=(const multimap& m) {
            _m_tree = m._m_tree;
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

        // Element access
        mapped_type&
        operator[] (const key_type& k)
        {
            iterator it = lower_bound(k);
            if (it == end() || key_comp()(k, it->first)) {
                it = insert(value_type(k, mapped_type()));
            }
            return it->second;
        }

        mapped_type&
        at(const key_type& k)
        {
            iterator it = lower_bound(k);
            if (it == end() || key_comp()(k, it->first)) {
                throw std::out_of_range("multimap::at");
            }
            return it->second;
        }

        const mapped_type&
        at(const key_type& k) const
        {
            iterator it = lower_bound(k);
            if (it == end() || key_comp()(k, it->first)) {
                throw std::out_of_range("multimap::at");
            }
            return it->second;
        }

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
        swap(multimap& x) { _m_tree.swap(x._m_tree); }

        void
        clear() { _m_tree.clear(); }

        // Observers
        key_compare
        key_comp() const
        {
            return _m_tree.key_comp();
        }

        value_compare
        value_comp() const
        {
            return value_compare(_m_tree.key_comp());
        }

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
        template <class Key2, class T2, class Compare2>
        friend bool
        operator==(const multimap<Key2, T2, Compare2>&, const multimap<Key2, T2, Compare2>&);

        template <class Key2, class T2, class Compare2>
        friend bool
        operator<(const multimap<Key2, T2, Compare2>&, const multimap<Key2, T2, Compare2>&);
    };

    template <class Key, class T, class Compare>
    inline bool
    operator==(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        return x._m_tree == y._m_tree;
    }

    template <class Key, class T, class Compare>
    inline bool
    operator!=(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        return !(x == y);
    }

    template <class Key, class T, class Compare>
    inline bool
    operator<(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        return x._m_tree < y._m_tree;
    }

    template <class Key, class T, class Compare>
    inline bool
    operator>(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        return y < x;
    }

    template <class Key, class T, class Compare>
    inline bool
    operator<=(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        return !(y < x);
    }

    template <class Key, class T, class Compare>
    inline bool
    operator>=(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        return !(x < y);
    }

    template <class Key, class T, class Compare>
    inline void
    swap(const multimap<Key, T, Compare>& x, const multimap<Key, T, Compare>& y)
    {
        x.swap(y);
    }

}

#endif