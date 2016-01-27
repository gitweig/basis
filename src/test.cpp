#include "basis_var.h"
#include <iostream>
#include "basis_thread.h"
#include "basis_sync_queue.h"
#include "basis_event.h"
#include "basis_str_tool.h"

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
	uint32 n = 100000;
	char* p = "di da di da  dida mao %d";
	
	SYSTEMTIME _time;
	GetSystemTime(&_time);
	uint32 st = _time.wSecond * 1000 + _time.wMilliseconds;
	
	for (uint32 i = 0; i < n; ++i)
	{
		sprintf();
	}

	return 0;
}
