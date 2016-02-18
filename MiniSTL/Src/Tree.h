/*
** Tree.h
** Created by Rayn on 2015/11/13
** red black tree
*/
#ifndef _TREE_H_
#define _TREE_H_

#include "Iterator.h"
#include "Allocator.h"
#include "Pair.h"

namespace rayn {

    enum __rb_tree_color { _s_red = false, _s_black = true };

    struct __rb_tree_node_base {
        typedef __rb_tree_color         color_type;
        typedef __rb_tree_node_base*    base_ptr;

        color_type  color;
        base_ptr    parent;
        base_ptr    left;
        base_ptr    right;

        static base_ptr minimum(base_ptr x) {
            while (x->left != 0) x = x->left;
            return x;
        }

        static base_ptr maximum(base_ptr x) {
            while (x->right != 0) x = x->right;
            return x;
        }
    };

    template <class Value>
    struct __rb_tree_node : public __rb_tree_node_base {
        typedef __rb_tree_node<Value>*  link_type;
        Value value_field;
    };

    struct __rb_tree_base_iterator {
        typedef __rb_tree_node_base::base_ptr   base_ptr;
        typedef bidirectional_iterator_tag      iterator_category;
        typedef ptrdiff_t                       difference_type;

        base_ptr node;

        void increment() {
            if (node->right != 0) {
                node = node->right;
                while (node->left != 0) {
                    node = node->left;
                }
            } else {
                base_ptr pa = node->parent;
                while (node == pa->right) {
                    node = pa;
                    pa = node->parent;
                }
                if (node->right != pa) {
                    node = pa;
                }
            }
        }

        void decrement() {
            if (node->color == _s_red &&
                node == node->parent->parent) {
                node = node->right;
            } else if (node->left != 0) {
                base_ptr temp = node->left;
                while (temp->right != 0) {
                    temp = temp->right;
                }
                node = temp;
            } else {
                base_ptr pa = node->parent;
                while (node == pa->left) {
                    node = pa;
                    pa = node->parent;
                }
                node = pa;
            }
        }
    };

    template <class Value, class Ref, class Ptr>
    struct __rb_tree_iterator : public __rb_tree_base_iterator {
        typedef Value   value_type;
        typedef Ref     reference;
        typedef Ptr     pointer;

        typedef __rb_tree_iterator<Value, Ref, Ptr>                     self;
        typedef __rb_tree_iterator<Value, Value&, Value*>               iterator;
        typedef __rb_tree_iterator<Value, const Value&, const Value*>   const_iterator;
        typedef __rb_tree_node<Value>*                                  link_type;

        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) { node = x; }
        __rb_tree_iterator(const iterator& it) { node = it.node; }

        reference operator* () const {
            return link_type(node)->value_field;
        }
        pointer operator-> () const {
            return &(operator*());
        }
        self& operator++ () {
            increment();
            return *this;
        }
        self operator++ (int) {
            self temp = *this;
            increment();
            return temp;
        }
        self& operator-- () {
            decrement();
            return *this;
        }
        self operator-- (int) {
            self temp = *this;
            decrement();
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
        typedef void*                       void_pointer;
        typedef __rb_tree_node_base*        base_ptr;
        typedef __rb_tree_node<Value>       rb_tree_node;
        typedef allocator<rb_tree_node>     rb_tree_node_allocator;
        typedef __rb_tree_color             color_type;

    public:
        typedef Key                 key_type;
        typedef Value               value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef rb_tree_node*       link_type;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;

    protected:
        link_type get_node() {
            return rb_tree_node_allocator::allocate();
        }
        void put_node(link_type p) {
            rb_tree_node_allocator::deallocate(p);
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

        link_type clone_node(link_type x) {
            link_type tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }

        void destroy_node(link_type p) {
            rayn::destroy(&p->value_field);
            put_node(p);
        }

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

    public:
        typedef __rb_tree_iterator<value_type, reference, pointer>  iterator;

    private:
        iterator    __insert(base_ptr _pos, base_ptr _pa, const value_type& v);
        link_type   __copy(link_type pos, link_type pa);
        void        __erase(link_type x);

        void init() {
            header = get_node();
            color(header) = _s_red;

            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }

    public:
        rb_tree(const Compare& comp = Compare()) : 
            node_count(0), key_compare(comp) { init(); }

        ~rb_tree() {
            put_node(header);
        }

        rb_tree<Key, Value, KeyOfValue, Compare>&
            operator= (const rb_tree<Key, Value, KeyOfValue, Compare>& other);

        Compare     key_comp() const    { return key_compare; }
        iterator    begin()             { return leftmost(); }
        iterator    end()               { return header; }
        bool        empty()             { return node_count == 0; }
        size_type   size() const        { return node_count; }
        size_type   max_size() const    { return size_type(-1); }

        pair<iterator, bool> insert_unique(const value_type& v);
        iterator insert_equal(const value_type& v);
    };

    inline void
    __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {

    }

    inline void
    __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {

    }
    
    inline void
    __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {

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
        if (key_compare(key(iter.node), KeyOfValue()(v))) {
            return pair<iterator, bool>(__insert(cur, pa, v), true);
        }
        return pair<iterator, bool>(iter, false);
    }

}

#endif

