/*
** Tree.h
** Created by Rayn on 2015/11/13
** red black tree
*/
#ifndef _TREE_H_
#define _TREE_H_

#include "Allocator.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "Pair.h"
#include "AlgoBase.h"

namespace rayn {

    enum __rb_tree_color { _s_red = false, _s_black = true };

    struct __rb_tree_node_base {
        typedef __rb_tree_color             color_type;
        typedef __rb_tree_node_base*        base_ptr;
        typedef const __rb_tree_node_base*  const_base_ptr;

        color_type  color;
        base_ptr    parent;
        base_ptr    left;
        base_ptr    right;

        static base_ptr
        minimum(base_ptr x) {
            while (x->left != 0) x = x->left;
            return x;
        }

        static const_base_ptr
        minimum(const_base_ptr x) {
            while (x->left != 0) x = x->left;
            return x;
        }

        static base_ptr
        maximum(base_ptr x) {
            while (x->right != 0) x = x->right;
            return x;
        }

        static const_base_ptr
        maximum(const_base_ptr x) {
            while (x->right != 0) x = x->right;
            return x;
        }
    };

    template <class Value>
    struct __rb_tree_node : public __rb_tree_node_base {
        typedef __rb_tree_node<Value>*  link_type;
        Value value_field;

        Value* valptr() {
            return &value_field;
        }
        const Value* valptr() const {
            return &value_field;
        }
    };

    __rb_tree_node_base*
    _rb_tree_increment(__rb_tree_node_base* node) throw ();

    const __rb_tree_node_base*
    _rb_tree_increment(const __rb_tree_node_base* node) throw ();

    __rb_tree_node_base*
    _rb_tree_decrement(__rb_tree_node_base* node) throw ();

    const __rb_tree_node_base*
    _rb_tree_decrement(const __rb_tree_node_base* node) throw ();


    template <class T>
    struct __rb_tree_iterator {
        typedef T       value_type;
        typedef T&      reference;
        typedef T*      pointer;

        typedef bidirectional_iterator_tag      iterator_category;
        typedef ptrdiff_t                       difference_type;

        typedef __rb_tree_iterator<T>           self;
        typedef __rb_tree_node_base::base_ptr   base_ptr;
        typedef __rb_tree_node<T>*              link_type;

        base_ptr _m_node;

        __rb_tree_iterator() : _m_node() {}

        explicit
        __rb_tree_iterator(base_ptr x) : _m_node(x) {}

        reference operator* () const {
            return static_cast<link_type>(_m_node)->value_field;
        }
        pointer operator-> () const {
            return static_cast<link_type>(_m_node)->valptr();
        }
        self& operator++ () {
            _m_node = _rb_tree_increment(_m_node);
            return *this;
        }
        self operator++ (int) {
            self temp = *this;
            _m_node = _rb_tree_increment(_m_node);
            return temp;
        }
        self& operator-- () {
            _m_node = _rb_tree_decrement(_m_node);
            return *this;
        }
        self operator-- (int) {
            self temp = *this;
            _m_node = _rb_tree_decrement(_m_node);
            return temp;
        }
        bool operator== (const self& x) const {
            return _m_node == x._m_node;
        }
        bool operator!= (const self& x) const {
            return _m_node != x._m_node;
        }
    };

    template <class T>
    struct __rb_tree_const_iterator {
        typedef T               value_type;
        typedef const T&        reference;
        typedef const T*        pointer;

        typedef bidirectional_iterator_tag              iterator_category;
        typedef ptrdiff_t                               difference_type;

        typedef __rb_tree_const_iterator<T>             self;
        typedef __rb_tree_iterator<T>                   iterator;
        typedef __rb_tree_node_base::const_base_ptr     base_ptr;
        typedef const __rb_tree_node<T>*                link_type;

        base_ptr _m_node;

        __rb_tree_const_iterator()
            : _m_node() {}

        explicit
        __rb_tree_const_iterator(base_ptr x)
            : _m_node(x) {}

        __rb_tree_const_iterator(const iterator& it)
            : _m_node(it._m_node) {}

        iterator _const_cast() const {
            return iterator(const_cast<typename iterator::base_ptr>(_m_node));
        }

        reference operator* () const {
            return static_cast<link_type>(_m_node)->value_field;
        }
        pointer operator-> () const {
            return static_cast<link_type>(_m_node)->valptr();
        }
        self& operator++ () {
            _m_node = _rb_tree_increment(_m_node);
            return *this;
        }
        self operator++ (int) {
            self temp = *this;
            _m_node = _rb_tree_increment(_m_node);
            return temp;
        }
        self& operator--() {
            _m_node = _rb_tree_decrement(_m_node);
            return *this;
        }
        self operator--(int) {
            self temp = *this;
            _m_node = _rb_tree_decrement(_m_node);
            return temp;
        }
        bool operator== (const self& x) const {
            return _m_node == x._m_node;
        }
        bool operator!= (const self& x) const {
            return _m_node != x._m_node;
        }
    };

    template <class T>
    inline bool
    operator==(const __rb_tree_iterator<T>& x,
               const __rb_tree_const_iterator<T>& y)
    { return x._m_node == y._m_node; }

    template <class T>
    inline bool
    operator!=(const __rb_tree_iterator<T>& x,
               const __rb_tree_const_iterator<T>& y)
    { return x._m_node != y._m_node; }

    void
    _rb_tree_insert_and_rebalance(const bool insert_left,
                                  __rb_tree_node_base* x,
                                  __rb_tree_node_base* pa,
                                  __rb_tree_node_base& header);

    __rb_tree_node_base*
    _rb_tree_rebalance_for_erase(__rb_tree_node_base* const z,
                                 __rb_tree_node_base& header);


    template <class Key, class Value, class KeyOfValue, class Compare>
    class rb_tree {
    protected:
        typedef void*                           void_pointer;
        typedef __rb_tree_node_base             base_node;
        typedef __rb_tree_node_base*            base_ptr;
        typedef const __rb_tree_node_base*      const_base_ptr;
        typedef __rb_tree_node<Value>*          link_type;
        typedef const __rb_tree_node<Value>*    const_link_type;
        typedef __rb_tree_node<Value>           rb_tree_node;
        typedef allocator<rb_tree_node>         node_allocator;
        typedef __rb_tree_color                 color_type;
        

    public:
        typedef Key                         key_type;
        typedef Value                       value_type;
        typedef value_type*                 pointer;
        typedef const value_type*           const_pointer;
        typedef value_type&                 reference;
        typedef const value_type&           const_reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;

        // iterators typedefs
        typedef __rb_tree_iterator<value_type>          iterator;
        typedef __rb_tree_const_iterator<value_type>    const_iterator;
        typedef reverse_iterator_t<iterator>            reverse_iterator;
        typedef reverse_iterator_t<const_iterator>      const_reverse_iterator;

    protected:
        size_type   node_count;
        base_node   header;
        Compare     key_compare;

        base_ptr&       _m_root()               { return header.parent; }
        const_base_ptr  _m_root() const         { return header.parent; }
        base_ptr&       _m_leftmost()           { return header.left; }
        const_base_ptr  _m_leftmost() const     { return header.left; }
        base_ptr&       _m_rightmost()          { return header.right; }
        const_base_ptr  _m_rightmost() const    { return header.right; }

        link_type 
        _m_begin()
        { return static_cast<link_type>(header.parent); }

        const_link_type
        _m_begin() const
        { return static_cast<const_link_type>(header.parent); }

        link_type
        _m_end()
        { return static_cast<link_type>(&header); }

        const_link_type
        _m_end() const
        { return static_cast<const_link_type>(&header); }

        static const_reference
        _s_value(const_link_type x)
        { return x->value_field; }

        static const Key&
        _s_key(const_link_type x)
        { return KeyOfValue()(_s_value(x)); }

        static const_reference
        _s_value(const_base_ptr x)
        { return static_cast<const_link_type>(x)->value_field; }

        static const Key&
        _s_key(const_base_ptr x)
        { return KeyOfValue()(_s_value(x)); }

        static link_type
        _s_left(base_ptr x)
        { return static_cast<link_type>(x->left); }

        static const_link_type
        _s_left(const_base_ptr x)
        { return static_cast<const_link_type>(x->left); }

        static link_type
        _s_right(base_ptr x)
        { return static_cast<link_type>(x->right); }

        static const_link_type
        _s_right(const_base_ptr x)
        { return static_cast<const_link_type>(x->right); }

        static base_ptr _s_minimum(base_ptr x) {
            return __rb_tree_node_base::minimum(x);
        }
        static const_base_ptr _s_minimum(const_base_ptr x) {
            return __rb_tree_node_base::minimum(x);
        }
        static base_ptr _s_maximum(base_ptr x) {
            return __rb_tree_node_base::maximum(x);
        }
        static const_base_ptr _s_maximum(const_base_ptr x) {
            return __rb_tree_node_base::maximum(x);
        }

    private:
        // helper functions
        link_type get_node() {
            return node_allocator::allocate();
        }
        void put_node(link_type p) {
            node_allocator::deallocate(p);
        }
        link_type create_node(const value_type& x) {
            link_type tmp = get_node();
            try {
                rayn::construct(&tmp->value_field, x);
            } catch (...) {
                put_node(tmp);
            }
            return tmp;
        }
        link_type clone_node(const_link_type x) {
            link_type tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }
        void destroy_node(link_type p) {
            rayn::destroy(&p->value_field);
        }
        void drop_node(link_type p) {
            destroy_node(p);
            put_node(p);
        }
        void _m_initialize() {
            header.color = _s_red;
            header.parent = 0;
            header.left = &header;
            header.right = &header;
        }
        void _m_reset() {
            header.parent = 0;
            header.left = &header;
            header.right = &header;
            node_count = 0;
        }

        void
        _m_move_data(rb_tree&);

        pair<base_ptr, base_ptr>
        _m_get_insert_unique_pos(const key_type& k);

        pair<base_ptr, base_ptr>
        _m_get_insert_equal_pos(const key_type& k);

        iterator
        _m_insert(base_ptr x, base_ptr pa, const value_type& v);

        iterator
        _m_insert_lower(base_ptr pa, const value_type& v);

        iterator
        _m_insert_equal_lower(base_ptr pa, const value_type& v);

        link_type
        _m_copy(const_link_type x, base_ptr p);

        void
        _m_erase(link_type x);

        void
        _m_erase_aux(const_iterator pos);
        
        void
        _m_erase_aux(const_iterator first, const_iterator last);

        iterator
        _m_lower_bound(link_type x, base_ptr pos, const Key& k);

        const_iterator
        _m_lower_bound(const_link_type x, const_base_ptr pos, const Key& k) const;

        iterator
        _m_upper_bound(link_type x, base_ptr pos, const Key& k);

        const_iterator
        _m_upper_bound(const_link_type x, const_base_ptr pos, const Key& k) const;

    public:
        // constructor/destructor
        rb_tree(const Compare& comp = Compare())
        : header(), node_count(0), key_compare(comp) 
        { _m_initialize(); }

        rb_tree(const rb_tree& other)
        : header(), node_count(0), key_compare(other.key_compare)
        {
            if (other._m_root() != 0) {
                _m_root() = _m_copy(other._m_begin(), _m_end());
                _m_leftmost() = _s_minimum(_m_root());
                _m_rightmost() = _s_maximum(_m_root());
                node_count = other.node_count;
            }
        }

        rb_tree(rb_tree&& other)
        : header(), node_count(0), key_compare(other.key_compare)
        {
            if (other._m_root() != 0) {
                _m_move_data(other);
            }
        }

        ~rb_tree() {
            _m_erase(_m_begin());
        }

        rb_tree&
        operator= (const rb_tree& other);

        // Accessors.
        Compare
        key_comp() const
        { return key_compare; }

        iterator
        begin()
        { return iterator(header.left); }

        const_iterator 
        begin() const
        { return const_iterator(header.left); }

        iterator
        end() 
        { return iterator(&header); }

        const_iterator
        end() const
        { return const_iterator(&header); }

        reverse_iterator
        rbegin()
        { return reverse_iterator(end()); }

        const_reverse_iterator
        rbegin() const 
        { return const_reverse_iterator(end()); }

        reverse_iterator
        rend()
        { return reverse_iterator(begin()); }

        const_reverse_iterator
        rend() const
        { return const_reverse_iterator(begin()); }

        // capacity
        bool        empty() const       { return node_count == 0; }
        size_type   size() const        { return node_count; }
        size_type   max_size() const    { return size_type(-1); }
        void        swap(rb_tree&);

        // modifiers
        template <typename InputIterator>
        void
        assign_unique(InputIterator first, InputIterator last);

        template <typename InputIterator>
        void
        assign_equal(InputIterator first, InputIterator last);

        pair<iterator, bool>
        insert_unique(const value_type& v);

        iterator
        insert_equal(const value_type& v);

        template <typename InputIterator>
        void
        insert_unique(InputIterator first, InputIterator last);

        template <typename InputIterator>
        void
        insert_equal(InputIterator first, InputIterator last);

        iterator
        erase(const_iterator pos) {
            const_iterator ret = pos;
            ++ret;
            _m_erase_aux(pos);
            return ret._const_cast();
        }

        iterator
        erase(iterator pos) {
            iterator ret = pos;
            ++ret;
            _m_erase_aux(pos);
            return ret;
        }

        iterator
        erase(const_iterator first, const_iterator last) {
            _m_erase_aux(first, last);
            return last._const_cast();
        }

        size_type
        erase(const key_type& x);

        void
        erase(const key_type* first, const key_type* last);

        void
        clear() {
            _m_erase(_m_begin());
            _m_reset();
        }

        // find operations.
        iterator
        find(const key_type& k);

        const_iterator
        find(const key_type& k) const;

        size_type
        count(const key_type& k) const;

        iterator
        lower_bound(const key_type& k) {
            return _m_lower_bound(_m_begin(), _m_end(), k);
        }

        const_iterator
        lower_bound(const key_type& k) const {
            return _m_lower_bound(_m_begin(), _m_end(), k);
        }

        iterator
        upper_bound(const key_type& k) {
            return _m_upper_bound(_m_begin(), _m_end(), k);
        }

        const_iterator
        upper_bound(const key_type& k) const {
            return _m_upper_bound(_m_begin(), _m_end(), k);
        }

        pair<iterator, iterator>
        equal_range(const key_type& k);

        pair<const_iterator, const_iterator>
        equal_range(const key_type& k) const;
    };

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline bool
    operator==(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
               const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return x.size() == y.size() && rayn::equal(x.begin(), x.end(), y.begin());
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline bool
    operator<(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
              const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return rayn::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline bool
    operator!=(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
               const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return !(x == y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline bool
    operator>(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
              const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return y < x;
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline bool
    operator<=(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
               const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return !(y < x);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline bool
    operator>=(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
               const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return !(x < y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    inline void
    swap(const rb_tree<Key, Value, KeyOfValue, Compare>& x,
         const rb_tree<Key, Value, KeyOfValue, Compare>& y)
    {
        return x.swap(y);
    }

    // _m_move_data
    template <class Key, class Value, class KeyOfValue, class Compare>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_move_data(rb_tree& other) {
        _m_root() = other._m_root();
        _m_leftmost() = other._m_leftmost();
        _m_rightmost() = other._m_rightmost();
        _m_root()->parent = _m_end();

        other._m_root() = 0;
        other._m_leftmost() = other._m_end();
        other._m_rightmost() = other._m_end();
        node_count = other.node_count;
        other.node_count = 0;
    }

    // _m_get_insert_unique_pos
    template <class Key, class Value, class KeyOfValue, class Compare>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::base_ptr,
         typename rb_tree<Key, Value, KeyOfValue, Compare>::base_ptr>
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_get_insert_unique_pos(const key_type& k)
    {
        typedef pair<base_ptr, base_ptr> Result;
        link_type x = _m_begin();
        base_ptr y = _m_end();
        bool comp = true;
        while (x != 0) {
            y = x;
            comp = key_compare(k, _s_key(x));
            x = comp ? _s_left(x) : _s_right(x);
        }
        iterator yit = iterator(y);
        if (comp) {
            if (yit == begin()) {
                return Result(x, y);
            } else {
                --yit;
            }
        }
        if (key_compare(_s_key(yit._m_node), k)) {
            return Result(x, y);
        }
        return Result(yit._m_node, 0);
    }

    // _m_get_insert_equal_pos
    template <class Key, class Value, class KeyOfValue, class Compare>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::base_ptr,
         typename rb_tree<Key, Value, KeyOfValue, Compare>::base_ptr>
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_get_insert_equal_pos(const key_type& k)
    {
        typedef pair<base_ptr, base_ptr> Result;
        link_type x = _m_begin();
        base_ptr y = _m_end();
        while (x != 0) {
            y = x;
            x = key_compare(k, _s_key(x)) ? _s_left(x) : _s_right(x);
        }
        return Result(x, y);
    }


    // _m_insert
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_insert(base_ptr x, base_ptr pa, const value_type& v)
    {
        bool insert_left = (x != 0 || pa == _m_end()
                            || key_compare(KeyOfValue()(v), _s_key(pa)));

        link_type z = create_node(v);
        _rb_tree_insert_and_rebalance(insert_left, z, pa, header);
        ++node_count;
        return iterator(z);
    }

    // _m_insert_lower
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_insert_lower(base_ptr pa, const value_type& v)
    {
        bool insert_left = (pa == _m_end()
                            || !key_compare(_s_key(pa), KeyOfValue()(v)));

        link_type z = create_node(v);
        _rb_tree_insert_and_rebalance(insert_left, z, pa, header);
        ++node_count;
        return iterator(z);
    }

    // _m_insert_equal_lower
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_insert_equal_lower(base_ptr pa, const value_type& v)
    {
        link_type cur = _m_begin();
        link_type pa = _m_end();

        while (cur != 0) {
            pa = cur->parent;
            cur = !key_compare(_s_key(cur), KeyOfValue()(v)) ?
                  _s_left(cur) : _s_right(cur);
        }
        return _m_insert_lower(pa, v);
    }

    // _m_copy
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_copy(const_link_type x, base_ptr p) {
        // Structural copy
        link_type top = clone_node(x);
        top->parent = p;

        try {
            if (x->right) {
                top->right = _m_copy(_s_right(x), top);
            }
            p = top;
            x = _s_left(x);
            while (x != 0) {
                link_type y = clone_node(x);
                p->left = y;
                y->parent = p;
                if (x->right) {
                    y->right = _m_copy(_s_right(x), y);
                }
                p = y;
                x = _s_left(x);
            }
        } catch (...) {
            _m_erase(top);
        }
        return top;
    }

    // _m_erase
    template <class Key, class Value, class KeyOfValue, class Compare>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_erase(link_type x)
    {
        // Erase without rebalancing.
        while (x != 0) {
            _m_erase(_s_right(x));
            link_type y = _s_left(x);
            drop_node(x);
            x = y;
        }
    }

    // _m_erase_aux
    template <class Key, class Value, class KeyOfValue, class Compare>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_erase_aux(const_iterator pos)
    {
        base_ptr ret = _rb_tree_rebalance_for_erase
                        (const_cast<base_ptr>(pos._m_node), header);
        drop_node(static_cast<link_type>(ret));
        --node_count;
    }

    // _m_erase_aux
    template <class Key, class Value, class KeyOfValue, class Compare>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_erase_aux(const_iterator first, const_iterator last)
    {
        if (first == begin() && last == end()) {
            clear();
        } else {
            while (first != last) {
                erase(first++);
            }
        }
    }

    // _m_lower_bound
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_lower_bound(link_type x, base_ptr pos, const Key& k)
    {
        while (x != 0) {
            if (!key_compare(_s_key(x), k)) {
                pos = x, x = _s_left(x);
            } else {
                x = _s_right(x);
            }
        }
        return iterator(pos);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_lower_bound(const_link_type x, const_base_ptr pos, const Key& k) const
    {
        while (x != 0) {
            if (!key_compare(_s_key(x), k)) {
                pos = x, x = _s_left(x);
            } else {
                x = _s_right(x);
            }
        }
        return const_iterator(pos);
    }

    // _m_upper_bound
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_upper_bound(link_type x, base_ptr pos, const Key& k)
    {
        while (x != 0) {
            if (key_compare(k, _s_key(x))) {
                pos = x, x = _s_left(x);
            } else {
                x = _s_right(x);
            }
        }
        return iterator(pos);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    _m_upper_bound(const_link_type x, const_base_ptr pos, const Key& k) const
    {
        while (x != 0) {
            if (key_compare(k, _s_key(x))) {
                pos = x, x = _s_left(x);
            } else {
                x = _s_right(x);
            }
        }
        return const_iterator(pos);
    }

    // operator=
    template <class Key, class Value, class KeyOfValue, class Compare>
    rb_tree<Key, Value, KeyOfValue, Compare>&
    rb_tree<Key, Value, KeyOfValue, Compare>::
    operator= (const rb_tree& other)
    {
        if (this != &other) {
            clear();
            key_compare = other.key_compare;
            if (other._m_root() != 0) {
                _m_root() = _m_copy(other._m_begin(), _m_end());
                _m_leftmost() = _s_minimum(_m_root());
                _m_rightmost() = _s_maximum(_m_root());
                node_count = other.node_count;
            }
        }
        return *this;
    }

    // swap
    template <class Key, class Value, class KeyOfValue, class Compare>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    swap(rb_tree& t)
    {
        if (_m_root() == 0) {
            if (t._m_root() != 0) {
                _m_root() = t._m_root();
                _m_leftmost() = t._m_leftmost();
                _m_rightmost() = t._m_rightmost();
                _m_root()->parent = _m_end();
                node_count = t.node_count;
                t._m_reset();
            }
        } else if (t._m_root() == 0) {
            t._m_root() = _m_root();
            t._m_leftmost() = _m_leftmost();
            t._m_rightmost() = _m_rightmost();
            t._m_root()->parent = t._m_end();
            t.node_count = node_count;
            _m_reset();
        } else {
            rayn::swap(_m_root(), t._m_root());
            rayn::swap(_m_leftmost(), t._m_leftmost());
            rayn::swap(_m_rightmost(), t._m_rightmost());
            rayn::swap(node_count, t.node_count);
            _m_root()->parent = _m_end();
            t._m_root()->parent = t._m_end();
        }
        rayn::swap(key_compare, t.key_compare);
    }

    // assign_unique
    template <class Key, class Value, class KeyOfValue, class Compare>
    template <typename InputIterator>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    assign_unique(InputIterator first, InputIterator last)
    {
        clear();
        for (; first != last; ++first) {
            insert_unique(*first);
        }
    }

    // assign_equal
    template <class Key, class Value, class KeyOfValue, class Compare>
    template <typename InputIterator>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    assign_equal(InputIterator first, InputIterator last)
    {
        clear();
        for (; first != last; ++first) {
            insert_equal(*first);
        }
    }

    // insert_unique
    template <class Key, class Value, class KeyOfValue, class Compare>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare>::
    insert_unique(const value_type& v)
    {
        typedef pair<iterator, bool> Result;
        pair<base_ptr, base_ptr> insert_pos
            = _m_get_insert_unique_pos(KeyOfValue()(v));
        if (insert_pos.second) {
            return Result(_m_insert(insert_pos.first, insert_pos.second, v),
                          true);
        }
        return Result(iterator(insert_pos.first), false);
    }

    // insert_equal
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    insert_equal(const value_type& v)
    {
        typedef pair<iterator, bool> Result;
        pair<base_ptr, base_ptr> insert_pos
            = _m_get_insert_equal_pos(KeyOfValue()(v));
        return _m_insert(insert_pos.first, insert_pos.second, v);
    }

    // insert_unique with range
    template <class Key, class Value, class KeyOfValue, class Compare>
    template <typename InputIterator>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    insert_unique(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first) {
            insert_unique(*first);
        }
    }

    // insert_equal with range
    template <class Key, class Value, class KeyOfValue, class Compare>
    template <typename InputIterator>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    insert_equal(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first) {
            insert_equal(*first);
        }
    }

    // erase
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare>::
    erase(const key_type& x)
    {
        pair<iterator, iterator> p = equal_range(x);
        const size_type old_size = size();
        erase(p.first, p.second);
        return old_size - size();
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    void
    rb_tree<Key, Value, KeyOfValue, Compare>::
    erase(const key_type* first, const key_type* last)
    {
        while (first != last) {
            erase(*first++);
        }
    }

    // find
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    find(const key_type& k) {
        iterator ret = _m_lower_bound(_m_begin(), _m_end(), k);
        return (ret == end() || key_compare(k, _s_key(ret._m_node))) ? end() : ret;
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    find(const key_type& k) const
    {
        const_iterator ret = _m_lower_bound(_m_begin(), _m_end(), k);
        return (ret == end() || key_compare(k, _s_key(ret._m_node))) ? end() : ret;
    }

    // count
    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare>::
    count(const key_type& k) const
    {
        pair<const_iterator, const_iterator> p = equal_range(k);
        const size_type n = rayn::distance(p.first, p.second);
        return n;
    }

    // equal_range
    template <class Key, class Value, class KeyOfValue, class Compare>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator,
         typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator>
    rb_tree<Key, Value, KeyOfValue, Compare>::
    equal_range(const key_type& k)
    {
        link_type cur = _m_begin();
        base_ptr pos = _m_end();

        while (cur != 0) {
            if (key_compare(_s_key(cur), k)) {
                cur = _s_right(cur);
            } else if (key_compare(k, _s_key(cur))) {
                pos = cur, cur = _s_left(cur);
            } else {
                link_type xu(cur);
                base_ptr yu(pos);
                pos = cur, cur = _s_left(cur);
                xu = _s_right(xu);
                return pair<iterator, iterator>(_m_lower_bound(cur, pos, k),
                                                _m_upper_bound(xu, yu, k));
            }
        }
        return pair<iterator, iterator>(iterator(pos), iterator(pos));
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator,
         typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator>
    rb_tree<Key, Value, KeyOfValue, Compare>::
    equal_range(const key_type& k) const
    {
        const_link_type cur = _m_begin();
        const_base_ptr pos = _m_end();

        while (cur != 0) {
            if (key_compare(_s_key(cur), k)) {
                cur = _s_right(cur);
            } else if (key_compare(k, _s_key(cur))) {
                pos = cur, cur = _s_left(cur);
            } else {
                const_link_type xu(cur);
                const_base_ptr yu(pos);
                pos = cur, cur = _s_left(cur);
                xu = _s_right(xu);
                return pair<const_iterator,
                            const_iterator>(_m_lower_bound(cur, pos, k),
                                            _m_upper_bound(xu, yu, k));
            }
        }
        return pair<const_iterator,
                    const_iterator>(const_iterator(pos),
                                    const_iterator(pos));
    }
}

#endif

