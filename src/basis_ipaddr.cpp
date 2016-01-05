#include "basis_ipaddr.h"
#include "basis_str_tool.h"
#include <WS2tcpip.h>

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
	uint32 numip = inet_addr(ip.c_str());
	if (numip == INADDR_NONE )
	{
		return ErrorIp;
	}

	in_addr addr;
	addr.S_un.S_addr = numip;
	return BSIpAddr(addr);		
}

basis::BSIpAddr BSIpAddr::make_ipaddr_by_num( uint32 ipnum )
{
	if (ipnum == INADDR_NONE)
	{
		return ErrorIp;
	}

	in_addr addr;
	addr.S_un.S_addr = htonl(ipnum);
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
	char ipaddr[16] = {0};
	sprintf(ipaddr, "%d.%d.%d.%d",
		m_ip_addr.S_un.S_un_b.s_b1,
		m_ip_addr.S_un.S_un_b.s_b2,
		m_ip_addr.S_un.S_un_b.s_b3,
		m_ip_addr.S_un.S_un_b.s_b4);

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
