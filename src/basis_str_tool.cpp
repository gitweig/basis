#include "basis_str_tool.h"

namespace basis
{
////////////////////////////////////////////////
// 字符串的相关操作
// class BSStrTool
////////////////////////////////////////////////
void BSStrTool::wipeHead(string& str, const char* ch /* = "" */)
{
	if (NULL == ch) return;

	size_t pos = str.find_first_not_of(ch);

	str.erase(0, pos);
}

void BSStrTool::wipeTail(string& str, const char* ch /* = "" */)
{
	if (NULL == ch) return;

	size_t pos = str.find_last_not_of(ch);
	if (string::npos == pos)
	{
		str.clear();
		return;
	}

	str.erase(++pos);
}

void BSStrTool::wipeMatching(string& str, const char* ch /* = "" */)
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

void BSStrTool::replaceChar(string& str, const char* oldChar, char newChar)
{
	if (NULL == oldChar) return;

	const char* pos = NULL;
	for (size_t i = 0; i < str.size(); ++i)
	{
		char& ch = str[i];
		pos = oldChar;
		while (*pos)
		{
			if (ch == *pos)
			{
				ch = newChar;
			}
			++pos;
		}
	}
}

string BSStrTool::strSplicing(const vector<string>& strList, const string& sep)
{
	size_t str_count = strList.size();
	size_t mem = sep.size() * (str_count - 1);

	for (size_t i = 0; i < str_count; ++i)
	{
		mem += strList[i].size();
	}

	string _result;
	_result.reserve(mem);

	for (size_t i = 0; i < str_count - 1; ++i)
	{
		_result += strList[i] + sep;
	}

	return _result + strList[str_count - 1];
}

void BSStrTool::split(const string& str, vector<string>& items,  const char* args, uint32 itemCount)
{
	size_t pos_begin = 0;
	size_t pos_end = 0;
	size_t _count = 1;
	const char* pos = str.c_str();

	while (string::npos != pos_end && _count <= itemCount)
	{
		pos_begin = str.find_first_not_of(args, pos_end);
		if (string::npos == pos_begin) return;

		pos_end = str.find_first_of(args, pos_begin);
		if (string::npos == pos_end && 0 == pos_begin)
		{
			// 找不到间隔字符，直接返回
			return;
		}

		string item = string(pos, pos_begin, pos_end - pos_begin);
		items.push_back(item);
		pos_begin = pos_end;
		++_count;
	}

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

		return result;
	}

	return result;
}

bool BSStrTool::toUint32(const string& str, uint32& uValue, uint32 radix)
{
	if (str.empty())
	{
		uValue = 0;
		return false;	
	}

	errno = 0;

	char *pEnd = NULL;
	string strValue = str;

	// 去除空格等符号
	wipeMatching(strValue, "\t\r\n ");

	uValue = strtoul(strValue.c_str(), &pEnd, radix);
	if (pEnd - strValue.c_str() != strValue.size())
	{
		return false;
	}

	if (ERANGE == errno)
	{
		return false;
	}

	return true;
}

bool BSStrTool::toInt32(const string& str, int32& nValue, uint32 radix /* = 10 */)
{
	if (str.empty())
	{
		nValue = 0;
		return false;	
	}

	errno = 0;

	char *pEnd = NULL;
	string strValue = str;

	// 去除空格等符号
	wipeMatching(strValue, "\t\r\n ");

	nValue = strtol(strValue.c_str(), &pEnd, radix);
	if (pEnd - strValue.c_str() != strValue.size())
	{
		return false;
	}

	if (ERANGE == errno)
	{
		return false;
	}

	return true;
}

bool BSStrTool::toDoulbe(const string& str, double& dValue, uint32 radix /* = 10 */)
{
	if (str.empty())
	{
		dValue = 0.0;
		return false;
	}

	errno = 0;

	char *pEnd = NULL;
	string strValue = str;

	// 去除空格等符号
	wipeMatching(strValue, "\t\r\n ");

	dValue = strtod(strValue.c_str(), &pEnd);
	if (pEnd - strValue.c_str() != strValue.size())
	{
		return false;
	}

	if (ERANGE == errno)
	{
		return false;
	}

	return true;
}

uint32 BSStrTool::getHashCode(const string& key)  
{  
	uint32 hashcode = 0;  
	uint32 kelength = (int) key.length();  

	uint32 COUNT = (kelength >> 2) + ((kelength & 3) ? 1 : 0);  

	uint32 _d = (COUNT << 2) - kelength;

	uint32* data = new uint32[kelength/4 + 4];  

	memcpy(((char*) data) + 0, key.c_str(), kelength);  
	memset(((char*) data) + kelength, kelength, (COUNT << 2) - kelength);  

	for (uint32 n = 0; n < COUNT; n++)  
	{  
		hashcode += (data[n] << n);  
	}  

	delete[] data;
	return hashcode;  
}  

uint32 BSStrTool::FNVHash(const string& str)   
{   
	uint32 fnv_prime = 0x811C9DC5;   
	uint32 hash = 0;   

	for(size_t i = 0; i < str.length(); i++)   
	{   
		hash *= fnv_prime;   
		hash ^= str[i];   
	}   

	return hash;
}

uint32 BSStrTool::murmur_hash2(const string& _data)
{
	uint32  h, k;

	const char* data = _data.c_str();
	size_t len = _data.size();

	h = 0 ^ len;

	while (len >= 4) {
		k  = data[0];
		k |= data[1] << 8;
		k |= data[2] << 16;
		k |= data[3] << 24;

		k *= 0x5bd1e995;
		k ^= k >> 24;
		k *= 0x5bd1e995;

		h *= 0x5bd1e995;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len) {
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= 0x5bd1e995;
	}

	h ^= h >> 13;
	h *= 0x5bd1e995;
	h ^= h >> 15;

	return h;
}

}