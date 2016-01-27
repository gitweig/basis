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
	// ȥ���ַ���ͷ��ָ���ַ�
	static void wipeHead(string& str, const char* ch = " \t\r\n");
	// ȥ���ַ���β��ָ���ַ�
	static void wipeTail(string& str, const char* ch = " \t\r\n");
	// ȥ���ַ���ƥ���ַ�
	static void wipeMatch(string& str, const char* ch = " \t\r\n");

	// �ַ���ת����Сд
	static void toUpper(string& str);
	static void toLower(string& str);

	// �滻ָ���ַ����е��ַ����ַ���
	static void replaceChar(string& str, const char* exceptedChar, char benchChar);
	static void replaceString(string& str, const char* oldStr, const char* newChar);

	// �ַ���ƴ��(���һ���ַ��������ӷָ��ַ���)
	static string splicing(const vector<string>& strList, const string& sep);
	static void splicing(string& desStr, const vector<string>& strList, const string& sep);

	// ���ַ�������ָ���ַ��ָ�
	static uint32 split(const string& str, vector<string>& items, const char* sep);

	// ��ʽ�����string
	static string sprintf(const char* format, ...);

	// ���ַ���ת�����޷�������
	static bool toUint32(const string& str, uint32& uValue, uint32 radix = 10);
	static uint32 toUint32Def(const string& str, uint32 defalut = 0, uint32 radix = 10);

	// ���ַ���ת�����з�������
	static bool toInt32(const string& str, int32& nValue, uint32 radix = 10);
	static uint32 toInt32Def(const string& str, int32 defalut = 0, uint32 radix = 10);
	
	// ���ַ���ת����double��
	static bool toDoulbe(const string& str, double& dValue);
	static double toDoulbeDef(const string& str, double defalut = 0.0);

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
	// uint8���� �ػ�
	static string toStr(uint8 _value)
	{
		ostringstream ostr;
		ostr.precision(numeric_limits<double>::digits10);
		ostr<<(uint32)_value;
		return ostr.str();
	}
};

};
using namespace basis;
#endif