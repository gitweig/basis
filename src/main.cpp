#include "basis_define.h"
#include "basis_json.h"
#include "basis_notification.h"
#include "basis_observer.h"
#include "basis_observer_ext.h"
#include "basis_subject.h"

void testJsonParse()
{
	string jsonStr = "{\"a\":\"Ìì\", \"b\": \"cc\"}";
	string jsonStr1 = "{\"key\":[4,6,311], \"tian\":{\"qi\": 999}}";
	BSJsonParser jsonParse;
	BSJsonValue jsonValue;
	if (!jsonParse.parse(jsonStr, jsonValue))
		return;
};

int main()
{


	return 0;
}