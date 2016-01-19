
#include "basis_atomic.h"

namespace basis 
{
#ifdef __WINDOWS__

BSAtomic32::BSAtomic32(int32 i)
	: m_counter(i)
{
	//
}

BSAtomic32::BSAtomic32(const BSAtomic32& from)
	: m_counter(from.get_value())
{
	//
}

BSAtomic32::~BSAtomic32()
{
	// empty
}

int32 BSAtomic32::get_value() const
{
	return InterlockedCompareExchange((volatile LONG*)&m_counter, 0, 0);
}

int32 BSAtomic32::set_value(int32 i)
{
	return InterlockedExchange((LONG*)&m_counter, i);
}

int32 BSAtomic32::test_zero_inc()
{
	for (; ;)
	{
		LONG tmp = get_value();
		if (tmp <= 0) return tmp;
		if (InterlockedCompareExchange((LONG*)&m_counter, tmp + 1, tmp) == tmp) return tmp + 1;
	}
	return 0;
}

BSAtomic32::operator int32()
{
	return get_value();
}

BSAtomic32& BSAtomic32::operator=(int32 i)
{
	this->set_value(i);
	return *this;
}

BSAtomic32& BSAtomic32::operator=(const BSAtomic32& from)
{
	if (this != &from)
	{
		this->set_value(from.get_value());
	}
	return * this;
}

int32 BSAtomic32::operator+= (int32 i)
{
	return InterlockedExchangeAdd((LONG*)&m_counter, i) + i;
}

int32 BSAtomic32::operator-= (int32 i)
{
	return InterlockedExchangeAdd((LONG*)&m_counter, -i) - i;
}

int32 BSAtomic32::operator++ (int)
{
	return  InterlockedIncrement((LONG*)&m_counter) - 1;
}

int32 BSAtomic32::operator-- (int)
{
	return InterlockedDecrement((LONG*)&m_counter) + 1;
}

int32 BSAtomic32::operator++ ()
{
	return InterlockedIncrement((LONG*)&m_counter);
}

int32 BSAtomic32::operator-- ()
{
	return InterlockedDecrement((LONG*)&m_counter);
}

#endif//__WINDOWS__

///////////////////////////////////////////////////////////////////////////////

#ifdef __POSIX__

BSAtomic32::BSAtomic32(int32 i)
	: m_counter(i)
{
	ASSERT(sizeof(int32) == sizeof(int32_t));
}

BSAtomic32::BSAtomic32(const BSAtomic32& from)
	: m_counter(from.get_value())
{
	ASSERT(sizeof(int32) == sizeof(int32_t));
}

BSAtomic32::~BSAtomic32()
{
	// empty
}

int32 BSAtomic32::get_value() const
{
	return __sync_val_compare_and_swap((volatile int*)&m_counter, 0, 0);
}

int32 BSAtomic32::set_value(int32 i)
{
	// set m_counter = i and return old value of m_counter
	return __sync_lock_test_and_set(&m_counter, i);
}

int32 BSAtomic32::test_zero_inc()
{
	// if (m_counter != 0) ++m_counter; return m_counter;

	for (; ;)
	{
		int32 tmp = get_value();
		if (tmp <= 0) return tmp;
		if (__sync_val_compare_and_swap(&m_counter, tmp, tmp + 1) == tmp) return tmp + 1;
	}
	return 0;
}

BSAtomic32::operator int32()
{
	return get_value();
}

BSAtomic32& BSAtomic32::operator= (int32 i)
{
	this->set_value(i);
	return *this;
}

BSAtomic32& BSAtomic32::operator= (const BSAtomic32& from)
{
	if (this != &from)
	{
		this->set_value(from.get_value());
	}
	return *this;
}

int32 BSAtomic32::operator+= (int32 i)
{
	return __sync_add_and_fetch(&m_counter, i);
}

int32 BSAtomic32::operator-= (int32 i)
{
	return __sync_sub_and_fetch(&m_counter, i);
}

int32 BSAtomic32::operator++ (int)
{
	return __sync_fetch_and_add(&m_counter, 1);
}

int32 BSAtomic32::operator-- (int)
{
	return __sync_fetch_and_sub(&m_counter, 1);
}

int32 BSAtomic32::operator++ ()
{
	return __sync_add_and_fetch(&m_counter, 1);
}

int32 BSAtomic32::operator-- ()
{
	return __sync_sub_and_fetch(&m_counter, 1);
}

#endif//__POSIX__

}//namespace basis