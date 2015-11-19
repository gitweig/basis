#include "basis_define.h"
#include "basis_notification.h"
#include "basis_observer.h"
#include "basis_observer_ext.h"
#include "basis_subject.h"
#include "basis_md5.h"
#include <iostream>
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
	string str("a");
	MD5 md5;	
	cout << md5.update(str) << endl;

	return 0;
}