#include "basis_time.h"

namespace basis
{
// time_t --> struct tm
static bool XTime_LocalTime(struct tm& _Tm, const time_t& _Time)
{
#ifdef __WINDOWS__
		return (0 == localtime_s(&_Tm, &_Time));
#endif//__WINDOWS__

#ifdef __POSIX__
		return (NULL != localtime_r(&_Time, &_Tm));
#endif// __POSIX__
}

///////////////////////////////////////////////////////////////////////////////
// class XTime
///////////////////////////////////////////////////////////////////////////////
XTime::XTime(void)
{
#ifdef __WINDOWS__
		SYSTEMTIME st = {0};
		GetLocalTime(&st);
		m_tm.tm_year  = st.wYear - 1900;
		m_tm.tm_mon   = st.wMonth - 1;
		m_tm.tm_mday  = st.wDay;
		m_tm.tm_hour  = st.wHour;
		m_tm.tm_min   = st.wMinute;
		m_tm.tm_sec   = st.wSecond;
		m_tm.tm_isdst = -1;
		m_sec         = mktime(&m_tm);
		m_usec        = st.wMilliseconds * 1000;
#endif//__WINDOWS__

#ifdef __POSIX__
		struct timeval tv_ = {};
		int ret = gettimeofday(&tv_, NULL);
		ASSERT(0 == ret);
		m_sec = tv_.tv_sec;
		m_usec = tv_.tv_usec;
		bool bRet = XTime_LocalTime(m_tm, m_sec);
		ASSERT(bRet);
#endif//__POSIX__
}

XTime::XTime( const XTime& other )
{
		memcpy(this, &other, sizeof(XTime));
}

XTime::XTime( time_t sec, long usec /*= 0*/ )
{

}

XTime::~XTime(void)
{
	// empty
}

void XTime::set( time_t sec, long usec /*= 0*/ )
{
}

} //namespace basis