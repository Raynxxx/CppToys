/*
** Type.h
** Created by Rayn on 2016/02/18
*/
#ifndef _REFLECT_TYPE_H_
#define _REFLECT_TYPE_H_

namespace reflect {
    // just a empty type
    struct NotType {};

    // type supported by reflection
    enum Type {
        notType,
        boolType,
        intType,
        floatType,
        stringType,
        enumType,
        arrayType,
        classType
    };
}

#endif