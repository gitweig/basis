#include "basis_var.h"
#include <iostream>

using namespace basis;

int main()
{
	BSVar str(string("123445"));
	BSVar tt(10);

	map<string, BSVar> map_;
	map_["str"] = str;
	map_["tt"] = tt;
	BSVar map_value(map_);

	bool ismap = map_value.isMap();

	const map<string, BSVar>& d = map_value.extract_def< map<string, BSVar> >();

	return 0;
}
