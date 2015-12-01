#include "basis_thread.h"
#include "basis_checked_delete.h"
#include "basis_event.h"

#ifdef __WINDOWS__
#include <process.h>
#endif //__WINDOWS__

namespace basis 
{

#ifdef __WINDOWS__

class BSThread::BSThreadImpl
{
public:
	BSThreadImpl(const string& name);
	~BSThreadImpl();

public:
	bool start(BSRunnable* run, BSThread* pinterface);
	void stop();
	void join();
	bool wait_quit(uint32 ms);

	bool started();
	uint32 thread_id();
	const string& name();

private:
	static uint32 __stdcall start_trn(void * arg);

private:
	string m_name;
	uint32 m_tid;
	BSEvent m_event;
	HANDLE  m_handle;
	BSThread* m_interface;
	BSRunnable* m_runnable;
	BSAtomic32  m_started;
};

BSThread::BSThreadImpl::BSThreadImpl( const string& name )
	: m_name(name)
	, m_tid(0)
	, m_event(true)
	, m_handle(INVALID_HANDLE_VALUE)
	, m_interface(NULL)
	, m_runnable(NULL)
{
}

BSThread::BSThreadImpl::~BSThreadImpl()
{
	ASSERT(m_started.get_value() == 0);
}

bool BSThread::BSThreadImpl::start( BSRunnable* runnable, BSThread* pinterface )
{
	do 
	{
		if (m_started++ != 0) break;
		if (!(m_interface = pinterface)) break;
		if (runnable) m_runnable = runnable;

		m_handle = (HANDLE)_beginthreadex(NULL, 0, &BSThreadImpl::start_trn, this, 0, &m_tid);
		
		ASSERT(m_handle != INVALID_HANDLE_VALUE);
		if (m_handle != INVALID_HANDLE_VALUE) 
		{
			return true;
		}
	} while (false);

	m_started--;
	return false;
}

void BSThread::BSThreadImpl::stop()
{
	m_event.set();
}

void BSThread::BSThreadImpl::join()
{
	WaitForSingleObject(m_handle, INFINITE);
	CloseHandle(m_handle);
	m_tid = 0;
	m_event.reset();
	m_interface = NULL;
	m_runnable = NULL;
	m_handle = INVALID_HANDLE_VALUE;
	VERIFY(m_started-- > 0);
}

bool BSThread::BSThreadImpl::wait_quit( uint32 ms )
{
	return m_event.wait(ms);
}

bool BSThread::BSThreadImpl::started()
{
	return m_started.get_value() > 0;
}

uint32 BSThread::BSThreadImpl::thread_id()
{
	return m_tid;
}

const string& BSThread::BSThreadImpl::name()
{
	return m_name;
}

uint32 __stdcall BSThread::BSThreadImpl::start_trn( void * arg )
{
	BSThreadImpl* impl = static_cast<BSThreadImpl*>(arg);
	if (impl == NULL) return 0;

	if (impl->m_runnable)
	{
		impl->m_runnable->run(impl->m_interface);
	}
	else
	{
		impl->m_interface->run(impl->m_interface);
	}
	return 0;
}

#endif //__WINDOWS__

#ifdef __POSIX__

class BSThread::BSThreadImpl
{
public:
	BSThreadImpl(const string& name);
	~BSThreadImpl();

public:
	bool start(BSRunnable* run, BSThread* pinterface);
	void stop();
	void join();
	bool wait_quit(uint32 ms);

	bool started();
	uint32 thread_id();
	const string& name();

private:
	static void* start_trn(void * arg);

private:
	string m_name;
	pthread_t m_tid;
	BSEvent m_event;
	BSThread* m_interface;
	BSRunnable* m_runnable;
	BSAtomic32  m_started;
};

BSThread::BSThreadImpl::BSThreadImpl(const string& name)
	: m_name(name)
	, m_event(true)
	, m_interface(NULL)
	, m_runnable(NULL)	
{
}

BSThread::BSThreadImpl::~BSThreadImpl()
{
	ASSERT(m_started.get_value() == 0);
}

bool BSThread::BSThreadImpl::start( BSRunnable* runnable, BSThread* pinterface)
{
	do 
	{
		if (m_started++ != 0) break;
		if (!(m_interface = pinterface)) break;
		if (runnable) m_runnable = runnable;

		int result = pthread_create(&m_tid, NULL, &BSThread::start_trn, pthread);

		ASSERT(result == 0);	
		if (result == 0)
		{
			return true;
		}
	} while (false);

	m_started--;
	return false;
}

void BSThread::BSThreadImpl::stop()
{
	m_event.set();
}

void BSThread::BSThreadImpl::join()
{
	VERIFY(pthread_join(m_tid, NULL));
	m_event.reset();
	m_interface = NULL;
	m_runnable = NULL;
	m_handle = INVALID_HANDLE_VALUE;
	VERIFY(m_started-- > 0);	
}

void* BSThread::BSThreadImpl::start_trn( void * arg )
{
	if (impl == NULL) return 0;

	if (impl->m_runnable)
	{
		impl->m_runnable->run(impl->m_interface);
	}
	else
	{
		impl->m_interface->run(impl->m_interface);
	}
	return NULL;
}

bool BSThread::BSThreadImpl::wait_quit(uint32 ms)
{
	return m_event.wait(ms);
}

bool BSThread::BSThreadImpl::started()
{
	return m_started.get_value() > 0;
}

uint32 BSThread::BSThreadImpl::thread_id()
{
	return (uint32)m_tid;
}

const string& BSThread::BSThreadImpl::name()
{
	return m_name;
}

#endif //__POSIX__

BSThread::BSThread( const string& name )
	: m_impl(new(nothrow) BSThread::BSThreadImpl(name))
{
}

BSThread::~BSThread()
{
	checked_delete(m_impl);
}

bool BSThread::start( BSRunnable* runnable /*= NULL*/ )
{
	return m_impl->start(runnable, this);
}

void BSThread::stop()
{
	return m_impl->stop();
}

void BSThread::join()
{
	return m_impl->join();
}

bool BSThread::wait_quit( uint32 ms )
{
	return m_impl->wait_quit(ms);
}

bool BSThread::started()
{
	return m_impl->started();
}

uint32 BSThread::thread_id()
{
	return m_impl->thread_id();
}

const string& BSThread::name()
{
	return m_impl->name();
}

}; //namespace basis