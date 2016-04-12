#include "basis_timeformat.h"
#include "basis_timestamp.h"

namespace basis
{

bool BSTimeFormat::do_parse( BSTimeStamp& timestamp,const string& strTime )
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
			return false;
		}
	}

	if ((tm_.tm_year < 1900) ||
		(tm_.tm_mon == 0 || tm_.tm_mon > 12) ||
		(tm_.tm_mday == 0 || tm_.tm_mday > 31) ||
		(tm_.tm_hour > 23 || tm_.tm_min > 59 || tm_.tm_sec > 59))
	{
		return false;
	}

	tm_.tm_year -= 1900;
	tm_.tm_mon -= 1;
	tm_.tm_isdst = -1; // 不了解情况时为负
	timestamp.set(tm_);
	return true;
}

string BSTimeFormat::format_time( BSTimeStamp& timestamp, int32 style )
{
	string strtime;
	const time_t timenow = (time_t)timestamp.get() / 1000000;
	strtime = BSTimeFormat::format_time(timenow, style);
	return strtime;
}

string BSTimeFormat::format_time( const time_t timestamp, int32 style )
{
	struct tm* ptm;
	ptm = localtime(&timestamp);

	char str[64];
	switch (style)
	{
	case YMD_0:
		sprintf(str, "%04d-%02d-%02d",
			ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
		break;
	case HMS_0:
		sprintf(str, "%02d:%02d:%02d",
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		break;
	default:
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d",
			ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		break;
	}

	return str;
}
////////////////////////////////////////////////////////////////////////////////////
BSTimeFormat::BSTimeFormat()
{
	//empty
}

BSTimeFormat::BSTimeFormat( time_t timestamp )
	: m_timestamp(timestamp,0)
{
	//empty
}

BSTimeFormat::BSTimeFormat( const BSTimeStamp& timestamp )
	: m_timestamp(timestamp)
{
	//empty
}

}// namespace basis