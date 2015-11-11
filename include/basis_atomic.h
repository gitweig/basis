// 2015-11-10
// basis_atomic.h
// 
// 原子操作类

#ifndef _BASIS_ATOMIC_H
#define _BASIS_ATOMIC_H

#include "basis_define.h"

namespace basis
{
///////////////////////////////////////////////////////////////////////////////
// class XAtomic32
///////////////////////////////////////////////////////////////////////////////
class XAtomic32
{
public:
	XAtomic32(int32 i = 0);
	XAtomic32(const XAtomic32& from);
	~XAtomic32();

	int32 get_value() const;  // 返回当前值
	int32 set_value(int32 i); // 返回原值
	int32 test_zero_inc(); // 返回增量后的值

	operator int32 ();
	XAtomic32& operator=(int32 i);
	XAtomic32& operator=(const XAtomic32& from);

	int32 operator+= (int32 i); // 返回增量后的值
	int32 operator-= (int32 i); // 返回减量后的值
	int32 operator++ (int); // 返回原值
	int32 operator-- (int); // 返回原值
	int32 operator++ (); // 返回增量后的值
	int32 operator-- (); // 返回减量后的值

private:
	volatile int32 m_counter;
};
} //namespace basis

using namespace basis;

#endif //_BASIS_ATOMIC_

