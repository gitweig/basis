#ifndef _BASIS_STR_TOOL_H_
#define _BASIS_STR_TOOL_H_

#include "basis_define.h"
#include <limits>

namespace basis
{
////////////////////////////////////////////////
// 字符串的相关操作
// class BSStrTool
////////////////////////////////////////////////
class BSStrTool
{
public:
	BSStrTool() {}
	~BSStrTool() {}

public:
	// 去除字符串头部指定字符
	static void wipeHead(string& str, const char* ch = "\t\r\n");
	// 去除字符串尾部指定字符
	static void wipeTail(string& str, const char* ch = "\t\r\n");
	// 去除字符串匹配字符
	static void wipeMatching(string& str, const char* ch = "\t\r\n");

	// 字符串转换成大写
	static void toUpper(string& str);
	// 字符串转换成小写
	static void toLower(string& str);

	// 替换指定字符集中的字符
	static void replaceChar(string& str, const char* oldChar, char newChar);

	// 字符串拼接(最后一个字符串不连接分隔字符串)
	static string strSplicing(const vector<string>& strList, const string& sep);

	// 将字符串按照指定字符分割
	static void split(const string& str, vector<string>& items, const char* args = "\t\r\n", uint32 itemCount = -1);
	// 格式化
	static string sprintf(const char* format, ...);
	// 将字符串转换成无符号整型
	static bool toUint32(const string& str, uint32& uValue, uint32 radix = 10);
	// 将字符串转换成有符号整型
	static bool toInt32(const string& str, int32& nValue, uint32 radix = 10);
	// 将字符串转换成double型
	static bool toDoulbe(const string& str, double& dValue, uint32 radix = 10);

	// 转换成字符串
	template<typename T>
	static string toStr(T _value)
	{	
		ostringstream ostr;
		// 统一设置成double 10进制显示精度
		ostr.precision(numeric_limits<double>::digits10);
		ostr<<_value;

		return ostr.str();
	}
	// 无符号类型转换成字符串
	static string toStr(uint8 _value)
	{
		char buff[10];
		itoa(_value, buff, 10);

		return buff;
	}

	// 字符串获得hash编码
	uint32 getHashCode(const string& str);
	// FNVHash编码
	uint32 FNVHash(const string& str);
	// murmur_hash2编码(最快)
	uint32 murmur_hash2(const string&);
};

};
using namespace basis;
#endif