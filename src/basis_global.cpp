#include "basis_ipaddr.h"
#include "basis_sockaddr.h"

namespace basis
{

#ifdef __WINDOWS__
class WSAStarter
{
public:
	WSAStarter()
	{
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int nRetCode = WSAStartup(wVersionRequested, &wsaData);
		ASSERTMSG((!nRetCode), "WSAStartup failed!");
	}

	~WSAStarter()
	{
		WSACleanup();
	}
};
static WSAStarter __wsa_starter;
#endif//__WINDOWS__

const basis::BSIpAddr BSIpAddr::AnyIp(INADDR_ANY);
const basis::BSIpAddr BSIpAddr::ErrorIp(INADDR_NONE);

const basis::BSSockAddr BSSockAddr::AnyAddr(BSIpAddr::AnyIp, 0);
const basis::BSSockAddr BSSockAddr::ErrorAddr(BSIpAddr::ErrorIp, 0);

}//namespace basis
