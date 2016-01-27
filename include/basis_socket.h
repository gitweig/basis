#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "basis_define.h"
#include "basis_sockaddr.h"
#include "basis_timespan.h"
#include "basis_ipaddr.h"

namespace basis
{
class BSSocket
{
public:
	BSSocket() : m_sock(INVALID_SOCKET) {}
	BSSocket(SOCKET sock) : m_sock(sock) {}
	~BSSocket();

public:
	operator SOCKET ();
	operator SOCKET () const; 

	bool open(int tp);
	bool close();
	bool isopen();
	
	void swap(BSSocket& sock);

	bool attach(SOCKET sock);
	SOCKET detach();

public:
	BSSockAddr local_addr() const;
	BSSockAddr remote_addr() const;

public:
	bool set_nonblock(bool bl = true);

	bool set_send_bufsize(uint32 sz);
	bool set_recv_bufsize(uint32 sz);
	bool get_send_bufsize(uint32& sz) const;
	bool get_recv_bufsize(uint32& sz) const;

public:
	int sock_error(); // 非阻塞无读写操作时 只能调用该函数获取错误码
	int last_error();
	const char* error_msg(int err);

private:
	SOCKET m_sock;
};

} // namespace xcore

using namespace basis;

#endif//_XCORE_SOCKET_H_
