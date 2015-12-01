#ifndef _BASIS_TIME_H_
#define _BASIS_TIME_H_

#include "basis_define.h"

namespace basis
{
//////////////////////////////////////////////////////////////////////////
// class XTime
//////////////////////////////////////////////////////////////////////////
class XTime
{
public:
	XTime(void);
	XTime(const XTime& other);
	XTime(time_t sec, long usec = 0);
	~XTime(void);

	void set(time_t sec, long usec = 0);

private:
	time_t           m_sec;  // local UTC time(second)
	long             m_usec; // local UTC time(microsecond)
	struct tm        m_tm;   // local time calendar
};

}

using namespace basis;
#endif