/*
** Set.h
** Created by Rayn on 2016/02/19
*/
#ifndef _SET_H_
#define _SET_H_

#include "Tree.h"
#include "Functional.h"

namespace rayn {

    template <class T, class Compare = rayn::less<T>>
    class set {
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
        typedef typename _rep_type::const_pointer       pointer;
        typedef typename _rep_type::const_pointer       const_pointer;
        typedef typename _rep_type::const_reference     reference;
        typedef typename _rep_type::const_reference     const_reference;
        typedef typename _rep_type::const_iterator      iterator;
        typedef typename _rep_type::const_iterator      const_iterator;
    };
}

#endif