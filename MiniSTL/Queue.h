/*
** Queue.h
** Created by Rayn on 2015/06/08
*/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <functional>

#include "Deque.h"
#include "Vector.h"
#include "Heap.h"

namespace rayn {

    // queue
    template <class T, class Sequence = deque<T> >
    class queue {
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
        queue()                             : c() {}
        // Contructor with a Sequence
        explicit queue(const Sequence& s)   : c(s) {}
        // Copy Contructor
        queue(const queue& other)           : c(other.c) {}
        // Move Contructor
        queue(queue&& other)                : c(other.c) {}
        // Destructor, none
        ~queue() {}

        // Copy Assignment operator
        queue& operator= (const queue& other);
        // Move Assignment operator
        queue& operator= (queue&& other);

        // Element access
        reference       front()         { return c.front(); }
        const_reference front() const   { return c.front(); }
        reference       back()          { return c.back(); }
        const_reference back() const    { return c.back(); }

        // Capacity
        bool            empty() const   { return c.empty(); }
        size_type       size() const    { return c.size(); }

        // Modifiers
        void            push(const T& value)    { c.push_back(value); }
        void            pop()                   { c.pop_front(); }
        void            swap(queue& other)      { rayn::swap(this->c, other.c); }

    protected:
        template <class T, class Sequence>
        friend bool operator== (queue<T, Sequence>&, queue<T, Sequence>&);

        template <class T, class Sequence>
        friend bool operator< (queue<T, Sequence>&, queue<T, Sequence>&);
    };

    template <class T, class Sequence>
    bool operator== (queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        return lhs.c == rhs.c;
    }
    template <class T, class Sequence>
    bool operator!= (queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        return !(lhs == rhs);
    }
    template <class T, class Sequence>
    bool operator<= (queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        return !(rhs < lhs);
    }
    template <class T, class Sequence>
    bool operator< (queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        return lhs.c < rhs.c;
    }
    template <class T, class Sequence>
    bool operator>= (queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        return !(lhs < rhs);
    }
    template <class T, class Sequence>
    bool operator> (queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        return rhs < lhs;
    }
    template <class T, class Sequence>
    void swap(queue<T, Sequence>& lhs, queue<T, Sequence>& rhs) {
        lhs.swap(rhs);
    }

    // priority_queue
    template <class T, class Sequence = vector<T>,
              class Compare = std::less<typename Sequence::value_type>>
    class priority_queue {
    public:
        typedef typename Sequence::value_type       value_type;
        typedef typename Sequence::size_type        size_type;
        typedef typename Sequence::reference        reference;
        typedef typename Sequence::const_reference  const_reference;

    protected:
        Sequence c;
        Compare comp;

    public:
        priority_queue()                                            : c() {}
        explicit priority_queue(const Compare& __comp)              : c(), comp(__comp) {}
        priority_queue(const Compare& __comp, const Sequence& __c)  : c(__c), comp(__comp) 
        {
            make_heap(__c.begin(), __c.end(), comp);
        }

        void pop() {
            pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
    };
}
#endif