#include "basis_ipaddr.h"
#include "basis_str_tool.h"
#ifdef __WINDOWS__
#include <WS2tcpip.h>
#endif

namespace basis
{

////////////////////////////////////////////////////////////////////////////////
// class BSIpAddr
////////////////////////////////////////////////////////////////////////////////
basis::BSIpAddr BSIpAddr::make_ipaddr_by_host( const string& host )
{
	addrinfo* info = NULL;
	if (getaddrinfo(host.c_str(), NULL, NULL, &info) < 0)
	{
		return ErrorIp;
	}

	in_addr ret_addr = ((struct sockaddr_in *)(info->ai_addr))->sin_addr;
	freeaddrinfo(info);

	return BSIpAddr(ret_addr);
}

basis::BSIpAddr BSIpAddr::make_ipaddr_by_ip( const string& ip )
{
	in_addr addr;
	if (inet_pton(AF_INET, ip.c_str(), &addr) != 1)
	{
		return ErrorIp;
	}
	return BSIpAddr(addr);		
}

basis::BSIpAddr BSIpAddr::make_ipaddr_by_num( uint32 ipnum )
{
	if (ipnum == INADDR_NONE)
	{
		return ErrorIp;
	}

	in_addr addr;
	addr.s_addr = htonl(ipnum);
	return BSIpAddr(addr);
}

basis::BSIpAddr BSIpAddr::make_ipaddr_by_sockaddr( const sockaddr_in& sock_addr )
{
	return BSIpAddr(sock_addr.sin_addr);
}

BSIpAddr::BSIpAddr( const BSIpAddr& r )
	: m_ip_addr(r.m_ip_addr)
{

}

BSIpAddr::BSIpAddr( uint32 ipnum )
{
	m_ip_addr.s_addr = ipnum;
}

const BSIpAddr& BSIpAddr::operator=( const BSIpAddr& r )
{
	m_ip_addr = r.m_ip_addr;
	return *this;
}

bool BSIpAddr::is_error() const
{
	return m_ip_addr.s_addr == INADDR_NONE;
}

std::string BSIpAddr::to_str() const
{
	string tmp_ip;
	to_str(tmp_ip);
	return tmp_ip;
}

const char* BSIpAddr::to_str( string& ip ) const
{
	union ConvUnion
	{
		struct { uint8 s_b1,s_b2,s_b3,s_b4; } un_b;
		uint32 un_l;
	} conv;
	conv.un_l = m_ip_addr.s_addr;

	char ipaddr[16] = {0};
	sprintf(ipaddr, "%d.%d.%d.%d",
		conv.un_b.s_b1,
		conv.un_b.s_b2,
		conv.un_b.s_b3,
		conv.un_b.s_b4);

	ip = ipaddr;
	return ip.c_str();
}

uint32 BSIpAddr::to_num() const
{
	return ntohl(m_ip_addr.s_addr);
}

bool BSIpAddr::operator==( const BSIpAddr& r )
{
	return m_ip_addr.s_addr == r.m_ip_addr.s_addr;
}

bool BSIpAddr::to_sockaddr_in( sockaddr_in& sock_addr ) const
{
	if (is_error()) return false;
	sock_addr.sin_addr = m_ip_addr;
	return true;
}

}//namespace basis
