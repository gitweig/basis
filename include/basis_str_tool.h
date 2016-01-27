#ifndef _BASIS_STR_TOOL_H_
#define _BASIS_STR_TOOL_H_

#include "basis_define.h"
#include <limits>

namespace basis
{
////////////////////////////////////////////////
// �ַ�������ز���
// class BSStrTool
////////////////////////////////////////////////
class BSStrTool
{
public:
	BSStrTool() {}
	~BSStrTool() {}

public:
	// ȥ���ַ���ͷ��ָ���ַ�
	static void wipeHead(string& str, const char* ch = "\t\r\n");
	// ȥ���ַ���β��ָ���ַ�
	static void wipeTail(string& str, const char* ch = "\t\r\n");
	// ȥ���ַ���ƥ���ַ�
	static void wipeMatching(string& str, const char* ch = "\t\r\n");

	// �ַ���ת���ɴ�д
	static void toUpper(string& str);
	// �ַ���ת����Сд
	static void toLower(string& str);

	// �滻ָ���ַ����е��ַ�
	static void replaceChar(string& str, const char* oldChar, char newChar);

	// �ַ���ƴ��(���һ���ַ��������ӷָ��ַ���)
	static string strSplicing(const vector<string>& strList, const string& sep);

	// ���ַ�������ָ���ַ��ָ�
	static void split(const string& str, vector<string>& items, const char* args = "\t\r\n", uint32 itemCount = -1);
	// ��ʽ��
	static string sprintf(const char* format, ...);
	// ���ַ���ת�����޷�������
	static bool toUint32(const string& str, uint32& uValue, uint32 radix = 10);
	// ���ַ���ת�����з�������
	static bool toInt32(const string& str, int32& nValue, uint32 radix = 10);
	// ���ַ���ת����double��
	static bool toDoulbe(const string& str, double& dValue, uint32 radix = 10);

	// ת�����ַ���
	template<typename T>
	static string toStr(T _value)
	{	
		ostringstream ostr;
		// ͳһ���ó�double 10������ʾ����
		ostr.precision(numeric_limits<double>::digits10);
		ostr<<_value;

		return ostr.str();
	}
	// �޷�������ת�����ַ���
	static string toStr(uint8 _value)
	{
		char buff[10];
		itoa(_value, buff, 10);

		return buff;
	}

	// �ַ������hash����
	uint32 getHashCode(const string& str);
	// FNVHash����
	uint32 FNVHash(const string& str);
	// murmur_hash2����(���)
	uint32 murmur_hash2(const string&);
};

};
using namespace basis;
#endif