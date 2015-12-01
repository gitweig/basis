#ifndef _BASIS_TIME_H_
#define _BASIS_TIME_H_

#include "basis_define.h"

namespace basis
{
//////////////////////////////////////////////////////////////////////////
// class BSTime
//////////////////////////////////////////////////////////////////////////
class BSTime
{
public:
	enum Style
	{
		YMDHMS_,		/* 2010-9-29 9:12:8 */
		YMDHMS_0,		/* 2010-09-29 09:12:08 */
		YMD_,			/* 2010-9-29 */
		YMD_0,			/* 2010-09-29 */
		HMS_,			/* 9:12:8 */
		HMS_0,			/* 09:12:08 */
		YMDHMS,			/* 20100929091208 */
		YMD,			/* 20100929 */
		HMS,			/* 091208 */
		MD,				/* 0929 */
		YMDWHMS			/* 2010092903091208 */
	};

	static const BSTime ZeroTime;
	static const BSTime StartTime; // the process start time
	static const BSTime MaxTime;
	static const BSTime ErrorTime;
	// support two time format
	// format 1: 2008-03-12 15:01:12
	// format 2: 20080312150112
	static BSTime try_parse(const string& strTime);

public:
	BSTime(void);
	BSTime(const BSTime& other);
	BSTime(const timeval& tv);
	BSTime(const tm& tm_);
	BSTime(time_t sec, long usec = 0);
	~BSTime(void);

	void set(time_t sec, long usec = 0);
	void set(const timeval& tv);
	void set(const tm& tm_);

	// seconds since 1970-01-01 00:00:00
	time_t sec(void) const { return m_sec; }
	// microseconds since the second
	long usec(void) const { return m_usec; }
	// seconds after the minute - [0,59]
	int local_sec(void) const { return m_tm.tm_sec; }
	// minutes after the hour - [0,59]
	int local_min(void) const { return m_tm.tm_min; }
	// hours since midnight - [0,23]
	int local_hour(void) const { return m_tm.tm_hour; }
	// day of the month - [1,31]
	int local_mday(void) const { return m_tm.tm_mday; }
	// months since January - [1,12]
	int local_mon(void) const { return m_tm.tm_mon + 1; }
	// years - [1900, ]
	int local_year(void) const { return m_tm.tm_year + 1900; }
	// days since Sunday - [0,6]
	int local_wday(void) const { return m_tm.tm_wday; }
	// days since January 1 - [0,365]
	int local_yday(void) const { return m_tm.tm_yday; }

	bool   has_error() const;
	uint32 to_Int() const;  // such as 20110915
	string to_str(int style = YMDHMS_0) const;

public:
	BSTime& operator = (const BSTime& t);
	BSTime& operator = (const timeval& tv);
	BSTime& operator = (const tm& t);
	BSTime& operator = (const time_t& t);

	bool operator == (const BSTime& dateTime) const;	
	bool operator != (const BSTime& dateTime) const;	
	bool operator <  (const BSTime& dateTime) const;	
	bool operator <= (const BSTime& dateTime) const;	
	bool operator >  (const BSTime& dateTime) const;	
	bool operator >= (const BSTime& dateTime) const;

private:
	void _fill_error_time();

private:
	time_t           m_sec;  // local UTC time(second)
	long             m_usec; // local UTC time(microsecond)
	struct tm        m_tm;   // local time calendar
};

inline bool BSTime::operator == ( const BSTime& dateTime ) const
{
	return ((m_sec == dateTime.m_sec) && (m_usec == dateTime.m_usec));
}

inline bool BSTime::operator != ( const BSTime& dateTime ) const
{
	return ((m_sec != dateTime.m_sec) || (m_usec != dateTime.m_usec));
}

inline bool BSTime::operator > ( const BSTime& dateTime ) const
{
	return((m_sec > dateTime.m_sec) || ((m_sec == dateTime.m_sec) && (m_usec > dateTime.m_usec)));
}

inline bool BSTime::operator < ( const BSTime& dateTime ) const
{
	return ((m_sec < dateTime.m_sec) || ((m_sec == dateTime.m_sec) && (m_usec < dateTime.m_usec)));
}

inline bool BSTime::operator >= ( const BSTime& dateTime ) const
{
	return ((m_sec > dateTime.m_sec) ||((m_sec == dateTime.m_sec) && (m_usec >= dateTime.m_usec)));
}

inline bool BSTime::operator <= ( const BSTime& dateTime ) const
{
	return ((m_sec < dateTime.m_sec) || ((m_sec == dateTime.m_sec) && (m_sec <= dateTime.m_usec)));
}

}
using namespace basis;

#endif