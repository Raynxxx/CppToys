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
		new(ptr)T1(value);  // ���� T1::T1(value)
	}

	/*
	** destroy() ��һ���汾������һ��ָ��
	*/
	template<class T>
	inline void destroy(T* ptr) {
		ptr->~T();
	}

	//destroy() �ڶ����汾, �����������������˺����跨�ҳ�Ԫ�ص���ֵ���
	//�������� _type_traits<T> ��ȡ���ʵ���ʩ
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
