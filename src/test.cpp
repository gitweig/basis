#include "basis_var.h"
#include <iostream>

int main()
{
	uint8 t = 8, p = 9, q = 0;
	basis::BSVar aa(t);
	basis::BSVar bb(p);
	t = aa.extract_def<int8>();
	aa.extract(q);
	uint32 value = 0;
	bool bl = aa.convert(value);

	p = (int8)aa;

	string s = "aaa";
	basis::BSVar ss(s);
	ss.extract_def<uint32>();
	ss.extract_def<vector<uint32> >();

	map<uint32, string> kk;
	basis::BSVar tt(kk);

	aa.isInterger();

	ss = aa;

	return 0;
}
