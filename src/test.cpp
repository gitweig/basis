#include "basis_var.h"
#include <iostream>

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

int main()
{
	ASSERTMSG(0, "wrong number");
	VERIFYMSG(0,"null number");
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

	return 0;
}
	