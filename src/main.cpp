#include "basis_define.h"
#include "basis_notification.h"
#include "basis_observer.h"
#include "basis_observer_ext.h"
#include "basis_subject.h"
#include "basis_md5.h"
#include <iostream>
using namespace std;

int main()
{
	uint32 start_ = time(0);
	for (uint32 i = 0; i < 2000; ++i)
	{
		string str("abc");
		MD5 md5;
		md5.update(str);
		cout << md5.toString() << endl;
	}
	cout << (start_ - time(0)) << endl;
	return 0;
}