#include "basis_mutex.h"

namespace basis 
{
#ifdef __WINDOWS__

class MutexImpl
{
	friend class BSMutex;

	MutexImpl(bool fast);
	~MutexImpl();

	void lock();
	bool try_lock();
	bool try_lock(uint32 ms);
	void unlock();

	HANDLE m_handle;
};

MutexImpl::MutexImpl(bool fast)
{
	m_handle = CreateMutex(NULL, FALSE, NULL);
	ASSERT(m_handle);
}

MutexImpl::~MutexImpl()
{
	CloseHandle(m_handle);
	m_handle = 0;
}

void MutexImpl::lock()
{
	switch (WaitForSingleObject(m_handle, INFINITE))
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_ABANDONED:
	default:
		ASSERT(FALSE);
	}
}

bool MutexImpl::try_lock()
{
	return try_lock(0);
}

bool MutexImpl::try_lock( uint32 ms )
{
	switch (WaitForSingleObject(m_handle, ms))
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_ABANDONED:
		return false;
	case WAIT_TIMEOUT:
	default:
		ASSERT(FALSE);
		return false;
	}
	return false;
}

void MutexImpl::unlock()
{
	VERIFY(ReleaseMutex(m_handle));
}

#endif//__WINDOWS__

///////////////////////////////////////////////////////////////////////////////

#ifdef __POSIX__

class MutexImpl
{
	friend class BSMutex;
	friend class BSFastMutex;

	MutexImpl(bool fast);
	~MutexImpl();

	void lock();
	bool try_lock();
	bool try_lock(uint32 ms);
	void unlock();

	pthread_mutex_t m_mutex;
};

MutexImpl::MutexImpl(bool fast)
{
	pthread_mutexattr_t arrt;
	pthread_mutexattr_init(&arrt);
	if (fast)
	{
		pthread_mutexattr_settype(&arrt, PTHREAD_MUTEX_NORMAL);
	}
	else
	{
		pthread_mutexattr_settype(&arrt, PTHREAD_MUTEX_RECURSIVE);
	}
	pthread_mutex_init(&m_mutex, &arrt);
	pthread_mutexattr_destroy(&arrt);
}

MutexImpl::~MutexImpl()
{
	pthread_mutex_destroy(&m_mutex);
}

void MutexImpl::lock()
{
	pthread_mutex_lock(&m_mutex);
}

bool MutexImpl::try_lock()
{
	return pthread_mutex_trylock(&m_mutex) == 0;
}

bool MutexImpl::try_lock( uint32 ms )
{
	return false;
}

void MutexImpl::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

#endif//__POSIX__

///////////////////////////////////////////////////////////////////////////////

BSMutex::BSMutex()
{
	m_impl = new(nothrow) MutexImpl(false);
	ASSERT(m_impl);
}

BSMutex::~BSMutex()
{
	delete m_impl;
	m_impl = NULL;
}

void BSMutex::lock()
{
	m_impl->lock();
}

bool BSMutex::try_lock()
{
	return m_impl->try_lock();
}

bool BSMutex::try_lock( uint32 ms )
{
	return m_impl->try_lock(ms);
}

void BSMutex::unlock()
{
	return m_impl->unlock();
}

///////////////////////////////////////////////////////////////////////////////

#ifdef __POSIX__

BSFastMutex::BSFastMutex()
{
	m_impl = new(nothrow) MutexImpl(true);
	ASSERT(m_impl);
}

BSFastMutex::~BSFastMutex()
{
	delete m_impl;
	m_impl = NULL;
}

void BSFastMutex::lock()
{
	m_impl->lock();
}

bool BSFastMutex::try_lock()
{
	return m_impl->try_lock();
}

bool BSFastMutex::try_lock( uint32 ms )
{
	return m_impl->try_lock(ms);
}

void BSFastMutex::unlock()
{
	return m_impl->unlock();
}

#endif//__POSIX__ 


}//namespace basis