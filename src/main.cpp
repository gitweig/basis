#include "basis_define.h"
#include "basis_notification.h"
#include "basis_observer.h"
#include "basis_observer_ext.h"
#include "basis_subject.h"
#include "basis_smart_ptr.h"
#include "stdio.h"

class TestA
{
public:
	TestA() 
	{
		printf("construct\n");
	}

	~TestA()
	{
		printf("destruct\n");
	}
};

int main()
{
	basis::weak_ptr<TestA> b, c;
	{
		basis::shared_ptr<TestA> a = shared_ptr<TestA>(new TestA);
		b = a;
		c = b;
		bool expried = b.expired();
		bool expriedc = c.expired();
		int e = expried;
	}
	
	bool expried = b.expired();
	bool expriedc = c.expired();

	
	return 0;
}