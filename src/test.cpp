#include "basis_var.h"

int main()
{
	uint8 t = 8, p = 9;
	basis::BSVar aa(t);
	basis::BSVar bb(p);
	bb=aa;
	bb.type();

	map<uint32, string> kk;
	basis::BSVar tt(kk);

	aa.isInterger();
	return 0;
}
