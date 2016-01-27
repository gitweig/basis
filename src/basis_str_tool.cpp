#include "basis_str_tool.h"

namespace basis
{
////////////////////////////////////////////////
// 字符串的相关操作
// class BSStrTool
////////////////////////////////////////////////
void BSStrTool::wipeHead(string& str, const char* ch)
{
	if (NULL == ch) return;
	size_t pos = str.find_first_not_of(ch);
	str.erase(0, pos);
}

void BSStrTool::wipeTail(string& str, const char* ch)
{
	if (NULL == ch) return;
	size_t pos = str.find_last_not_of(ch);
	if (string::npos == pos)
	{
		str.clear();
	}
	else
	{
		str.erase(++pos);
	}	
}

void BSStrTool::wipeMatch(string& str, const char* ch)
{
	size_t pos_b = 0;
	size_t pos_e = 0;

	while (true)
	{
		pos_b = str.find_first_of(ch, pos_b);
		if (pos_b == string::npos) break;

		pos_e = str.find_first_not_of(ch, pos_b);

		str.erase(pos_b, pos_e - pos_b);
		if (string::npos == pos_e) break;
	}
}

void BSStrTool::toUpper(string& str)
{
	unsigned char code_interal = 'a' - 'A';

	for (size_t i = 0; i < str.size(); ++i)
	{
		char& ch = str[i];
		if (ch >= 'a' && ch <= 'z')
		{
			ch = ch - code_interal;
		}
	}
}

void BSStrTool::toLower(string& str)
{
	unsigned char code_interal = 'a' - 'A';
	for (size_t i = 0; i < str.size(); ++i)
	{
		char& ch = str[i];
		if (ch >= 'A' && ch <= 'Z')
		{
			ch = ch + code_interal;
		}
	}
}

void BSStrTool::replaceChar(string& str, const char* exceptedChar, char benchChar)
{
	if (NULL == exceptedChar) return;

	const char* pos = NULL;
	for (size_t i = 0; i < str.size(); ++i)
	{
		char& ch = str[i];
		pos = exceptedChar;
		while (*pos)
		{
			if (ch == *pos)
			{
				ch = benchChar;
				break;
			}
			++pos;
		}
	}
}

string BSStrTool::splicing(const vector<string>& strList, const string& sep)
{
	string result_str;
	splicing(result_str, strList, sep);
	return result_str;
}

void BSStrTool::splicing( string& desStr, const vector<string>& strList, const string& sep )
{
	if (strList.empty()) return;

	// 计算空间
	size_t str_count = strList.size();
	size_t mem = sep.size() * (str_count - 1);
	for (size_t i = 0; i < str_count; ++i)
	{
		mem += strList[i].size();
	}

	// 预开辟
	desStr.reserve(mem);

	// 拼接
	for (size_t i = 0; i < str_count-1; ++i)
	{
		desStr += strList[i] + sep;
	}
	desStr + strList[str_count-1];
}

uint32 BSStrTool::split(const string& str, vector<string>& items, const char* sep)
{
	size_t pos_begin = 0, pos_end = 0;
	const char* pos = str.c_str();

	while (string::npos != pos_end)
	{
		pos_begin = str.find_first_not_of(sep, pos_end);
		if (string::npos == pos_begin) break;

		pos_end = str.find_first_of(sep, pos_begin);
		if (string::npos == pos_end && 0 == pos_begin) break;

		string item = string(pos, pos_begin, pos_end - pos_begin);
		items.push_back(item);
	}

	return items.size();
}

string BSStrTool::sprintf(const char* format, ...)
{
	string result = "";
	for (uint32 i = 1; i <= 20; ++i)
	{
		va_list arg_start;
		va_start(arg_start, format);

		char* buff = (char*)malloc(1024 * i);
		uint32 ret = vsprintf(buff, format, arg_start);
		if (ret >= 1024 * i)
		{
			free(buff);
			va_end(arg_start);
			continue;
		}
		result.assign(buff, ret);

		free(buff);
		va_end(arg_start);
		break;
	}
	return result;
}

bool BSStrTool::toUint32(const string& str, uint32& uValue, uint32 radix)
{
	if (str.empty()) return false;

	errno = 0;
	char *pEnd = NULL;
	uValue = strtoul(str.c_str(), &pEnd, radix);
	if (ERANGE == errno) return false;
	if (pEnd - str.c_str() != str.size()) return false;

	return true;
}

bool BSStrTool::toInt32(const string& str, int32& nValue, uint32 radix)
{
	if (str.empty()) return false;

	errno = 0;
	char *pEnd = NULL;
	nValue = strtol(str.c_str(), &pEnd, radix);
	if (ERANGE == errno) return false;
	if (pEnd - str.c_str() != str.size()) return false;

	return true;
}

bool BSStrTool::toDoulbe(const string& str, double& dValue)
{
	if (str.empty()) return false;

	errno = 0;
	char *pEnd = NULL;
	dValue = strtod(str.c_str(), &pEnd);
	if (ERANGE == errno) return false;
	if (pEnd - str.c_str() != str.size()) return false;

	return true;
}

uint32 BSStrTool::toUint32Def( const string& str, uint32 defalut, uint32 radix )
{
	uint32 result = 0;
	if (!toUint32(str, result, radix))
	{
		result = defalut;
	}
	return result;
}

uint32 BSStrTool::toInt32Def( const string& str, int32 defalut, uint32 radix )
{
	int32 result = 0;
	if (!toInt32(str, result, radix))
	{
		result = defalut;
	}
	return result;
}

double BSStrTool::toDoulbeDef( const string& str, double defalut )
{
	double result = 0.0;
	if (!toDoulbe(str, result))
	{
		result = defalut;
	}
	return result;
}

}