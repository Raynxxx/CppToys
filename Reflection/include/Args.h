/*
** Args.h
** Created by Rayn on 2016/02/18
*/
#ifndef _REFLECT_ARGS_H_
#define _REFLECT_ARGS_H_

#include <vector>

namespace reflect {
    class Value;

    class Args {
    public:
        template <typename... V>
        Args(V... args) {
            init({ args... });
        }

        void init(std::initializer_list<Value> il) {
            _m_values = il;
        }

    private:
        std::vector<Value> _m_values;
    };
}

#endif