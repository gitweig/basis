#include "basis_var.h"
#include <iostream>
#include "basis_thread.h"

using namespace basis;

class AA
{
public:
	AA()
	{
		printf("c\n");
	}

	~AA()
	{
		printf("d\n");
	}

	int tt;
};

class Run1 : public BSRunnable
{
public:
	virtual void run(BSThread* p)
	{
		
	}
};

class Run2 : public BSRunnable
{
public:
	virtual void run(BSThread* p)
	{
		
	}
};

int main()
{
	{
		int32 a = 33;
		BSVar aa(a);
		BSVar acopy = aa;
		int32& c = acopy.extract_def<int32>();
		c = 5;
	}

 	BSVar str(string("123445"));
	BSVar tt(10);

	map<string, BSVar> map_;
	map_["str"] = str;
	map_["tt"] = tt;
	BSVar map_value(map_);

	bool ismap = map_value.isMap();

	const map<string, BSVar>& d = map_value.extract_def< map<string, BSVar> >();

	BSThread thread1("t1");
	BSThread thread2("t2");

	Run1 r1;
	Run2 r2;

	thread1.start(&r1);
	thread2.start(&r2);

	//thread1.stop();
	//thread2.stop();

	thread1.join();
	thread2.join();
	

	return 0;
}
	