/*
** Tree.h
** Created by Rayn on 2015/11/13
** red black tree
*/
#ifndef _TREE_H_
#define _TREE_H_

#include "Iterator.h"
#include "ReverseIterator.h"
#include "Allocator.h"
#include "Pair.h"

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

    inline __rb_tree_node_base*
    _rb_tree_increment(__rb_tree_node_base* node) throw () {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0) {
                node = node->left;
            }
        } else {
            __rb_tree_node_base* pa = node->parent;
            while (node == pa->right) {
                node = pa;
                pa = node->parent;
            }
            if (node->right != pa) {
                node = pa;
            }
        }
        return node;
    }

    inline const __rb_tree_node_base*
    _rb_tree_increment(const __rb_tree_node_base* node) throw () {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0) {
                node = node->left;
            }
        } else {
            __rb_tree_node_base* pa = node->parent;
            while (node == pa->right) {
                node = pa;
                pa = node->parent;
            }
            if (node->right != pa) {
                node = pa;
            }
        }
        return node;
    }

    inline __rb_tree_node_base*
    _rb_tree_decrement(__rb_tree_node_base* node) throw () {
        if (node->color == _s_red &&
            node == node->parent->parent) {
            node = node->right;
        } else if (node->left != 0) {
            __rb_tree_node_base* temp = node->left;
            while (temp->right != 0) {
                temp = temp->right;
            }
            node = temp;
        } else {
            __rb_tree_node_base* pa = node->parent;
            while (node == pa->left) {
                node = pa;
                pa = node->parent;
            }
            node = pa;
        }
        return node;
    }

    inline const __rb_tree_node_base*
    _rb_tree_decrement(const __rb_tree_node_base* node) throw () {
        if (node->color == _s_red &&
            node == node->parent->parent) {
            node = node->right;
        } else if (node->left != 0) {
            __rb_tree_node_base* temp = node->left;
            while (temp->right != 0) {
                temp = temp->right;
            }
            node = temp;
        } else {
            __rb_tree_node_base* pa = node->parent;
            while (node == pa->left) {
                node = pa;
                pa = node->parent;
            }
            node = pa;
        }
        return node;
    }


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
            return node == x.node;
        }
        bool operator!= (const self& x) const {
            return node != x.node;
        }
    };

    template <class Key, class Value, class KeyOfValue, class Compare>
    class rb_tree {
    protected:
        typedef void*                           void_pointer;
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
        link_type   header;
        Compare     key_compare;

        link_type& root() const         { return (link_type&)header->parent; }
        link_type& leftmost() const     { return (link_type&)header->left; }
        link_type& rightmost() const    { return (link_type&)header->right; }

        static link_type&   left(link_type x)   { return (link_type&)(x->left); }
        static link_type&   right(link_type x)  { return (link_type&)(x->right); }
        static link_type&   parent(link_type x) { return (link_type&)(x->parent); }
        static reference    value(link_type x)  { return x->value_field; }
        static const Key&   key(link_type x)    { return KeyOfValue()(value(x)); }
        static color_type&  color(link_type x)  { return (color_type&)(x->color); }

        static link_type&   left(base_ptr x)   { return (link_type&)(x->left); }
        static link_type&   right(base_ptr x)  { return (link_type&)(x->right); }
        static link_type&   parent(base_ptr x) { return (link_type&)(x->parent); }
        static reference    value(base_ptr x)  { return ((link_type)x)->value_field; }
        static const Key&   key(base_ptr x)    { return KeyOfValue()(value(link_type(x))); }
        static color_type&  color(base_ptr x)  { return (color_type&)(link_type(x)->color); }

        static link_type minimum(link_type x) {
            return (link_type)__rb_tree_node_base::minimum(x);
        }
        static link_type maximum(link_type x) {
            return (link_type)__rb_tree_node_base::maximum(x);
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
            p->~__rb_tree_node<Value>();
        }
        void drop_node(link_type p) {
            destroy_node(p);
            put_node(p);
        }
        void empty_initialize() {
            header = get_node();
            color(header) = _s_red;
            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }

        iterator    __insert(base_ptr _pos, base_ptr _pa, const value_type& v);
        link_type   __copy(const_link_type pos, base_ptr pa);
        void        __erase(link_type x);

    public:
        rb_tree(const Compare& comp = Compare())
            : node_count(0), key_compare(comp) 
        { empty_initialize(); }

        rb_tree(const rb_tree& other)
            : key_compare(other.key_compare)
        {
            if (other.root() != 0) {
            }
        }

        ~rb_tree() {
            put_node(header);
        }

        rb_tree<Key, Value, KeyOfValue, Compare>&
            operator= (const rb_tree<Key, Value, KeyOfValue, Compare>& other);

        // iterators 
        iterator begin() { 
            return iterator(header->left);
        }
        const_iterator begin() const {
            return const_iterator(header->left);
        }

        iterator end() {
            return iterator(header);
        }
        const_iterator end() const {
            return const_iterator(header);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        // capacity
        Compare     key_comp() const    { return key_compare; }
        bool        empty()             { return node_count == 0; }
        size_type   size() const        { return node_count; }
        size_type   max_size() const    { return size_type(-1); }

        // modifiers
        pair<iterator, bool>    insert_unique(const value_type& v);
        iterator                insert_equal(const value_type& v);

        // element access
        iterator                find(const key_type& k);
    };

    /*
    ** @brief       rb-tree rotate left
    ** @param cur   the node will rotate
    ** @param root  the root node of tree
    */
    inline void
    __rb_tree_rotate_left(__rb_tree_node_base* cur, __rb_tree_node_base*& root) {
        __rb_tree_node_base* rson = cur->right;
        cur->right = rson->left;
        if (rson->left != 0) {
            rson->left->parent = cur;
        }
        rson->parent = cur->parent;

        if (cur == root) {
            root = rson;
        } else if (cur == cur->parent->left) {
            cur->parent->left = rson;
        } else {
            cur->parent->right = rson;
        }
        rson->left = cur;
        cur->parent = rson;
    }

    inline void
    __rb_tree_rotate_right(__rb_tree_node_base* cur, __rb_tree_node_base*& root) {
        __rb_tree_node_base* lson = cur->left;
        cur->left = lson->right;
        if (lson->right != 0) {
            lson->right->parent = cur;
        }
        lson->parent = cur->parent;

        if (cur == root) {
            root = lson;
        } else if (cur == cur->parent->right) {
            cur->parent->right = lson;
        } else {
            cur->parent->left = lson;
        }
        lson->right = cur;
        cur->parent = lson;
    }

    /*
    ** @brief       rebalance rb-tree
    ** @param son   the node will be insert into tree
    ** @param root  the root node of tree
    */
    inline void
    __rb_tree_rebalance(__rb_tree_node_base* son, __rb_tree_node_base*& root) {
        son->color = _s_red;
        // from down to up
        while (son != root && son->parent->color == _s_red) {
            if (son->parent == son->parent->parent->left) {
                __rb_tree_node_base* uncle = son->parent->parent->right;
                if (uncle && uncle->color == _s_red) {
                    son->parent->color = _s_black;
                    uncle->color = _s_black;
                    son->parent->parent->color = _s_red;
                    son = son->parent->parent;
                } else {
                    if (son == son->parent->right) {
                        son = son->parent;
                        __rb_tree_rotate_left(son, root);
                    }
                    son->parent->color = _s_black;
                    son->parent->parent->color = _s_red;
                    __rb_tree_rotate_right(son->parent->parent, root);
                }
            } else {
                __rb_tree_node_base* uncle = son->parent->parent->left;
                if (uncle && uncle->color == _s_red) {
                    son->parent->color = _s_black;
                    uncle->color = _s_black;
                    son->parent->parent->color = _s_red;
                    son = son->parent->parent;
                } else {
                    if (son == son->parent->left) {
                        son = son->parent;
                        __rb_tree_rotate_right(son, root);
                    }
                    son->parent->color = _s_black;
                    son->parent->parent->color = _s_red;
                    __rb_tree_rotate_left(son->parent->parent, root);
                }
            }
        }
        root->color = _s_black;
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare>::
    __copy(const_link_type pos, base_ptr pa) {
        link_type top = clone_node(pos);
        top->parent = pa;
        try {

        } catch (...) {

        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    __insert(base_ptr _pos, base_ptr _pa, const value_type& v)
    {
        link_type pos = (link_type) _pos;
        link_type pa = (link_type) _pa;
        link_type tmp;

        if (pa == header || pos != 0 || key_compare(KeyOfValue()(v), key(pa))) {
            tmp = create_node(v);
            left(pa) = tmp;
            if (pa == header) {
                root() = tmp;
                rightmost() = tmp;
            } else if (pa == leftmost()) {
                leftmost() = tmp;
            }
        } else {
            tmp = create_node(v);
            right(pa) = tmp;
            if (pa == rightmost()) {
                rightmost() = tmp;
            }
        }
        parent(tmp) = pa;
        left(tmp) = 0;
        right(tmp) = 0;

        __rb_tree_rebalance(tmp, header->parent);
        ++node_count;
        return iterator(tmp);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    insert_equal(const value_type& v)
    {
        link_type pa = header;
        link_type cur = root();
        while (cur != 0) {
            pa = cur;
            cur = key_compare(KeyOfValue()(v), key(cur)) ? left(cur) : right(cur);
        }
        return __insert(cur, pa, v);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare>::
    insert_unique(const value_type& v)
    {
        link_type pa = header;
        link_type cur = root();
        bool comp = true;
        while (cur != 0) {
            pa = cur;
            comp = key_compare(KeyOfValue()(v), key(cur));
            cur = comp ? left(cur) : right(cur);
        }
        iterator iter = iterator(pa);
        if (comp) {
            if (iter == begin()) {
                return pair<iterator, bool>(__insert(cur, pa, v), true);
            } else {
                --iter;
                // InsertPoint's parent not min key, InsertValue need to
                // compare with prev key of InsertPoint's parent
            }
        }
        if (key_compare(key(iter._m_node), KeyOfValue()(v))) {
            return pair<iterator, bool>(__insert(cur, pa, v), true);
        }
        return pair<iterator, bool>(iter, false);
    }

    template <class Key, class Value, class KeyOfValue, class Compare>
    typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare>::
    find(const key_type& k) {
        rb_tree_node* last = header;
        rb_tree_node* cur = root();

        while (cur != 0) {
            if (!key_compare(key(cur), k)) {
                last = cur;
                cur = left(cur);
            } else {
                cur = right(cur);
            }
        }
        iterator last_it = iterator(last);
        return (last_it == end() || key_compare(k, key(last_it._m_node))) ? end() : last_it;
    }

}

#endif

