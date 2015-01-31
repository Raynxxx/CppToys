/*
** Construct.h
** Created by Rayn on 2015/01/08
*/
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>  // for placement new
#include "TypeTraits.h"

namespace rayn {

	template<class T1, class T2>
	inline void construct(T1* ptr, const T2& value) {
		new(ptr)T1(value);  // 调用 T1::T1(value)
	}

	/*
	** destroy() 第一个版本，接受一个指针
	*/
	template<class T>
	inline void destroy(T* ptr) {
		ptr->~T();
	}

	//destroy() 第二个版本, 接受两个迭代器，此函数设法找出元素的数值类别
	//进而利用 _type_traits<T> 求取最适当措施
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type);
	}

	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type) {}

	template<class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first != last; ++first) {
			destroy(&*first);
		}
	}
}

#endif
