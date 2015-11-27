#include "basis_event.h"
#include "basis_checked_delete.h"

namespace basis 
{

#ifdef __WINDOWS__

class BSEvent::BSEventImpl
{
public:
	~BSEventImpl();

private:
	friend class BSEvent;

	BSEventImpl( bool manual );

	bool setEvent();

	bool resetEvent();

	bool pulseEvent();

	bool wait( uint32 ms );

private:
	HANDLE m_handle;
};

BSEvent::BSEventImpl::BSEventImpl( bool manual )
{
	m_handle = CreateEvent(0, manual, false, 0);
	ASSERT(m_handle);
}

BSEvent::BSEventImpl::~BSEventImpl()
{
	VERIFY(CloseHandle(m_handle));
}

bool BSEvent::BSEventImpl::setEvent()
{
	return SetEvent(m_handle) ? true : false;
}

bool BSEvent::BSEventImpl::resetEvent()
{
	return ResetEvent(m_handle) ? true : false;
}

bool BSEvent::BSEventImpl::pulseEvent()
{
	return PulseEvent(m_handle) ? true : false;
}

bool BSEvent::BSEventImpl::wait( uint32 ms )
{
	switch (WaitForSingleObject(m_handle, ms))
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	case WAIT_FAILED:
	default:
		ASSERT(false);
		break;
	}
	return false;
}

#endif//__WINDOWS__

#ifdef __POSIX__



#endif// __POSIX__

BSEvent::BSEvent( bool manual /*= false*/ )
	: m_impl(new(nothrow) BSEventImpl(manual))
{
	ASSERT(m_impl);
}

BSEvent::~BSEvent()
{
	checked_delete(m_impl);
}

bool BSEvent::setEvent()
{
	if (m_impl == NULL) return false;
	return m_impl->setEvent();
}

bool BSEvent::resetEvent()
{
	if (m_impl == NULL) return false;
	return m_impl->resetEvent();
}

bool BSEvent::pulseEvent()
{
	if (m_impl == NULL) return false;
	return m_impl->pulseEvent();
}

bool BSEvent::wait( uint32 ms /*=-1*/ ) 
{
	if (m_impl == NULL) return false;
	return m_impl->wait(ms);
}

}//namespace basis