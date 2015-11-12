#include "basis_define.h"
#include "basis_notification.h"
#include "basis_observer.h"
#include "basis_observer_ext.h"
#include "basis_subject.h"

int main()
{
	BSSubject aaa;
	
	string a1 = "aaaaa";
	string a2 = "aaaaa";
	string a3 = a1;

	printf("%X\n", a1.c_str());
	printf("%X\n", a2.c_str());
	printf("%X\n", a3.c_str());

	return 0;
}