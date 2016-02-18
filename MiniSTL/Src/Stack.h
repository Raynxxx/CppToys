/*
** Stack.h
** Created by Rayn on 2015/06/08
*/
#ifndef _STACK_H_
#define _STACK_H_

#include "Deque.h"

namespace rayn {
    template <class T, class Sequence = deque<T> >
    class stack {
    public:
        typedef             Sequence                    container_type;
        typedef typename    Sequence::value_type        value_type;
        typedef typename    Sequence::size_type         size_type;
        typedef typename    Sequence::reference         reference;
        typedef typename    Sequence::const_reference   const_reference;

    protected:
        // the underlying container.
        Sequence c;

    public:
        // Default Contructor
        stack()                             : c() {}
        // Contructor with a Sequence
        explicit stack(const Sequence& s)   : c(s) {}
        // Copy Contructor
        stack(const stack& other)           : c(other.c) {}
        // Move Contructor
        stack(stack&& other)                : c(other.c) {}
        // Destructor, none
        ~stack() {}

        // Copy Assignment operator
        stack& operator= (const stack& other);
        // Move Assignment operator
        stack& operator= (stack&& other);

        // Element access
        reference       top()           { return c.back(); }
        const_reference top() const     { return c.back(); }

        // Capacity
        bool            empty() const   { return c.empty(); }
        size_type       size() const    { return c.size(); }

        // Modifiers
        void            push(const T& value)    { c.push_back(value); }
        void            pop()                   { c.pop_back();  }
        void            swap(stack& other)      { rayn::swap(this->c, other.c); }

    protected:
        template <class T, class Sequence>
        bool operator== (stack<T, Sequence>&, stack<T, Sequence>&);
        template <class T, class Sequence>
        bool operator< (stack<T, Sequence>&, stack<T, Sequence>&);
    };

    template <class T, class Sequence>
    bool operator== (stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        return lhs.c == rhs.c;
    }
    template <class T, class Sequence>
    bool operator!= (stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        return !(lhs == rhs);
    }
    template <class T, class Sequence>
    bool operator<= (stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        return !(rhs < lhs);
    }
    template <class T, class Sequence>
    bool operator< (stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        return lhs.c < rhs.c;
    }
    template <class T, class Sequence>
    bool operator>= (stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        return !(lhs < rhs);
    }
    template <class T, class Sequence>
    bool operator> (stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        return rhs < lhs;
    }
    template <class T, class Sequence>
    void swap(stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) {
        lhs.swap(rhs);
    }

}

#endif
