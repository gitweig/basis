#include "basis_time.h"

namespace basis
{
// time_t --> struct tm
static bool BSTime_LocalTime(struct tm& _Tm, const time_t& _Time)
{
#ifdef __WINDOWS__
		return (0 == localtime_s(&_Tm, &_Time));
#endif//__WINDOWS__

#ifdef __POSIX__
		return (NULL != localtime_r(&_Time, &_Tm));
#endif// __POSIX__
}

///////////////////////////////////////////////////////////////////////////////
// class BSTime
///////////////////////////////////////////////////////////////////////////////
const BSTime BSTime::ZeroTime(0, 0);

const BSTime BSTime::StartTime;

const BSTime BSTime::MaxTime((time_t)0x7fffffff, 999999);

const BSTime BSTime::ErrorTime(-1.1);

BSTime::BSTime(void)
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
		bool bRet = BSTime_LocalTime(m_tm, m_sec);
		ASSERT(bRet);
#endif//__POSIX__
}

BSTime::BSTime( const BSTime& other )
{
	memcpy(this, &other, sizeof(BSTime));
}

BSTime::BSTime(const timeval& tv)
{
	set(tv.tv_sec, tv.tv_usec);
}

BSTime::BSTime( time_t sec, long usec /*= 0*/ )
{
	set(sec, usec);
}

BSTime::BSTime( const tm& tm_ )
{
	set(tm_);
}

BSTime::~BSTime(void)
{
	// empty
}

void BSTime::set( time_t sec, long usec /*= 0*/ )
{
	sec += usec / 1000000;
	usec %= 1000000;
	if (sec > 0 && usec < 0)
	{
		sec--;
		usec += 1000000;
	}
	else if (sec < 0 && usec > 0)
	{
		sec++;
		usec -= 1000000;
	}
	if (usec < 0 || !BSTime_LocalTime(m_tm, sec))
	{
		_fill_error_time();
		return;
	}

	m_sec = sec;
	m_usec = usec;
	return;
}

void BSTime::set( const timeval& tv )
{
	set(tv.tv_sec, tv.tv_usec);
}

void BSTime::set( const tm& tm_ )
{
	m_sec = mktime((struct tm*)&tm_);
	m_usec = 0;
	m_tm = tm_;
	return;
}

void BSTime::_fill_error_time()
{
	m_sec = (time_t) - 1;
	m_usec = 0;
	memset(&m_tm, 0, sizeof(m_tm));
	return;
}

BSTime BSTime::try_parse( const string& strTime )
{
	struct tm tm_ = {};
	int ret = sscanf(strTime.c_str(), " %d - %d - %d %d : %d : %d",
		&tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
	if (ret != 6)
	{
		ret = sscanf(strTime.c_str(), "%4d%2d%2d%2d%2d%2d",
			&tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
		if (ret != 6)
		{
			return ErrorTime;
		}
	}

	if ((tm_.tm_year < 1900) ||
		(tm_.tm_mon == 0 || tm_.tm_mon > 12) ||
		(tm_.tm_mday == 0 || tm_.tm_mday > 31) ||
		(tm_.tm_hour > 23 || tm_.tm_min > 59 || tm_.tm_sec > 59))
	{
		return ErrorTime;
	}

	tm_.tm_year -= 1900;
	tm_.tm_mon -= 1;
	tm_.tm_isdst = -1; // 不了解情况时为负
	return BSTime(tm_);
}

bool BSTime::has_error() const
{
	return (m_sec == (time_t) - 1);
}

uint32 BSTime::to_Int() const
{
	if (has_error()) return 0;
	return local_year() * 10000 + local_mon() * 100 + local_mday();
}

string BSTime::to_str( int style /*= YMDHMS_0*/ ) const
{
	char str[64];

	if (has_error())
	{
		return "0000-00-00 00:00:00";
	}

	switch (style)
	{
	case YMDHMS_:
		sprintf(str, "%d-%d-%d %d:%d:%d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday,
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	case YMD_:
		sprintf(str, "%d-%d-%d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday);
		break;
	case YMD_0:
		sprintf(str, "%04d-%02d-%02d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday);
		break;
	case HMS_:
		sprintf(str, "%d:%d:%d",
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	case HMS_0:
		sprintf(str, "%02d:%02d:%02d",
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	case YMDHMS:
		sprintf(str, "%04d%02d%02d%02d%02d%02d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday,
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	case YMD:
		sprintf(str, "%04d%02d%02d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday);
		break;
	case HMS:
		sprintf(str, "%02d%02d%02d",
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	case MD:
		sprintf(str, "%02d%02d",
			m_tm.tm_mon + 1, m_tm.tm_mday);
		break;
	case YMDWHMS:
		sprintf(str, "%04d%02d%02d%02d%02d%02d%02d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday,
			m_tm.tm_wday == 0 ? 7 : m_tm.tm_wday,
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	case YMDHMS_0:
	default:
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d",
			m_tm.tm_year + 1900, m_tm.tm_mon + 1, m_tm.tm_mday,
			m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
		break;
	}
	return str;}

BSTime& BSTime::operator=( const BSTime& t )
{
	if (this != &t)
	{
		memcpy(this, &t, sizeof(t));
	}
	return *this;
}

BSTime& BSTime::operator=( const timeval& tv )
{
	set(tv.tv_sec, tv.tv_usec);
	return *this;
}

BSTime& BSTime::operator=( const tm& t )
{
	set(t);
	return *this;
}

BSTime& BSTime::operator=( const time_t& t )
{
	set(t, 0);
	return *this;
}

} //namespace basis