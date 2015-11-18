#include "basis_critical.h"

#ifdef __POSIX__
#include "basis_mutex.h"
#endif//__POSIX__

namespace basis 
{
#ifdef __WINDOWS__

class BSCritical::CriticalImpl
{
	friend class BSCritical;

	CriticalImpl();
	~CriticalImpl();

	void lock();
	bool try_lock();
	void unlock();
	
	CRITICAL_SECTION m_cs;
};

BSCritical::CriticalImpl::CriticalImpl()
{
	InitializeCriticalSection(&m_cs);
}

BSCritical::CriticalImpl::~CriticalImpl()
{
	DeleteCriticalSection(&m_cs);
}

void BSCritical::CriticalImpl::lock()
{
	EnterCriticalSection(&m_cs);
}

bool BSCritical::CriticalImpl::try_lock()
{
	return TryEnterCriticalSection(&m_cs) ? true : false;
}

void BSCritical::CriticalImpl::unlock()
{
	LeaveCriticalSection(&m_cs);
}

#endif//__WINDOWS__

///////////////////////////////////////////////////////////////////////////////

#ifdef __POSIX__

typedef BSMutex BSCritical;

#endif//__POSIX__

///////////////////////////////////////////////////////////////////////////////

BSCritical::BSCritical()
	: m_impl(new CriticalImpl())
{
	//ASSERT(m_impl);
}

BSCritical::~BSCritical()
{
	delete m_impl;
}

void BSCritical::lock()
{
	if (m_impl)
	{
		m_impl->lock();
	}
}

bool BSCritical::try_lock()
{
	if (m_impl)
	{
		return m_impl->try_lock();
	}
	return true;
}

void BSCritical::unlock()
{
	if (m_impl)
	{
		m_impl->unlock();
	}
}

}//namespace basis