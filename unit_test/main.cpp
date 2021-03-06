#include "basis_var.h"
#include <iostream>
#include "basis_thread.h"
#include "basis_sync_queue.h"
#include "basis_event.h"
#include "basis_str_tool.h"
#include "basis_ipaddr.h"
#include "basis_md5.h"
#include "gtest/gtest.h"

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

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	getchar();
	return 0;
}
