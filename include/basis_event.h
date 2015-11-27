#ifndef _BASIS_EVENT_H_
#define _BASIS_EVENT_H_

#include "basis_define.h"

namespace basis
{

class BSEvent
{
public:
	BSEvent(bool manual = false);

	~BSEvent();

public:
	bool setEvent();

	bool resetEvent();

	bool pulseEvent();

	bool wait(uint32 ms = -1);

private:
	BSEvent( const BSEvent& );
	BSEvent& operator=( const BSEvent& );

private:
	class BSEventImpl;
	BSEventImpl* m_impl;
};

// ∫¡√Î
void sleep(uint32 ms)
{
#ifdef __WINDOWS__
	Sleep(ms);
#elif __POSIX__
	usleep(ms * 1000);
#endif // __WINDOWS__

}

} //namespace basis

using namespace basis;

#endif //_BASIS_EVENT_H_

