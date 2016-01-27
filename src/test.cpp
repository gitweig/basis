#include "basis_var.h"
#include <iostream>
#include "basis_thread.h"
#include "basis_sync_queue.h"
#include "basis_event.h"
#include "basis_timeformat.h"

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

	time_t t = time(0);
	char tmp[64];
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A �����%j�� %z", localtime(&t) );

	SYSTEMTIME sys;
	GetLocalTime( &sys );
	printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n", sys.wYear,   sys.wMonth,  sys.wDay, sys.wHour, sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek);

	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nTime;
	QueryPerformanceFrequency(&m_nFreq); // ��ȡʱ������
	QueryPerformanceCounter(&m_nTime);//��ȡ��ǰʱ��
	printf(" time:%lld us",(m_nTime.QuadPart*1000000/m_nFreq.QuadPart));//m_nFreq.QuadPartΪ:����/s�������Ϳ��Ի�ú��뼶�����

	Sleep(10000);
	getchar();
	return 0;
}
	