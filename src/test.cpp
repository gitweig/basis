#include "basis_var.h"
#include <iostream>
#include "basis_thread.h"
#include "basis_sync_queue.h"
#include "basis_event.h"
#include "basis_buffer.h"

void test_buffer()
{
	BSBuffer buffer;

	buffer.fill_data("0123456789", 10);
	buffer.reserve(17);
	char* data = (char*)malloc(10);

	for (uint8 i = 0; i < 10; ++i)
	{
		buffer.take_data((void*)data, 5);
		data[5] = '\0';
		string str = data;
		printf("%s\n", str.c_str());
		buffer.fill_data("01234",5);

		bool k = buffer.take_data(data, 5);
		data[5] = '\0';
		string str1 = data;
		printf("%s\n", str1.c_str());
		buffer.fill_data("56789", 5);
	}

	buffer.print_data();

	bool k1 = buffer.take_data(data, 6);
	data[6] = '\0';
	string str2 = data;
	printf("%s\n", str2.c_str());
}

void test_mem()
{
	char *s="Golden Global View";
	char* d = (char*)malloc(1000);
	char* d1 = (char*)malloc(100);

	memcpy(d,s,10);
	memcpy(d1, d, 6);

	d[strlen(s)]=0;
	printf("%s",d);
}

int main()
{
	//test_mem();
	test_buffer();

	getchar();
	return 0;
}
	