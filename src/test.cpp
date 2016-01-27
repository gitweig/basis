#include "basis_var.h"
#include <iostream>
#include "basis_thread.h"
#include "basis_sync_queue.h"
#include "basis_event.h"
#include "basis_str_tool.h"
#include "basis_ipaddr.h"
#include "basis_md5.h"

using namespace basis;

BSSyncQueue<int> syncqueue;

BSEvent the_event;
BSAtomic32 the_count;

class Run1 : public BSRunnable
{
public:
	virtual void run(BSThread* p)
	{
		while (!p->wait_quit(0))
		{
			syncqueue.push(1);
			if (the_count++ > 20000000)
			{
				break;
			}
		}
	}
};

class Run2 : public BSRunnable
{
public:
	virtual void run(BSThread* p)
	{
		while (!p->wait_quit(0))
		{
			int value = 0;
			syncqueue.pull(value);
			if (syncqueue.size() == 0)
			{
				break;
			}
		}
	}
};


int main()
{
	double aaa = 986756.32323;
	uint16 bbb = 234;
	int32 ccc = -2335445;
	uint8 ddd = 65;
	char eee = 'a';
	string a = BSStrTool::toStr(aaa);
	string b = BSStrTool::toStr(bbb);
	string c = BSStrTool::toStr(ccc);
	string d = BSStrTool::toStr(ddd);
	string e = BSStrTool::toStr(eee);
	printf("%s\n", a.c_str());
	printf("%s\n", b.c_str());
	printf("%s\n", c.c_str());
	printf("%s\n", d.c_str());
	printf("%s\n", e.c_str());

	string ipaddr = "115.159.54.142";
	BSIpAddr addr = BSIpAddr::make_ipaddr_by_ip(ipaddr);
	string newip = addr.to_str();
	printf("%s\n", newip.c_str());
	string md5str = MD5().update(ipaddr);
	printf("%s\n", md5str.c_str());

	return 0;
}
