#ifndef _BASIS_BUFFER_H_
#define _BASIS_BUFFER_H_

#include "basis_define.h"

namespace basis
{
//////////////////////////////////////////////////////////////////////////
// 自定义缓冲区
// class BSBuffer
//////////////////////////////////////////////////////////////////////////
class BSBuffer
{
	enum { INIT_MEM = 1024, MAX_MEM = 1024 * 1024 };

public:
	BSBuffer();
	~BSBuffer();
	
	// 内存大小
	uint32 capatiy() { return m_size; }
	// 使用内存
	uint32 useSize() { return m_use_size; }

	// 填充数据
	bool fillData(void* pData, uint32 _size);
	// 提取数据
	bool takeData(void* pData, uint32 _size);

private:
	uint32 m_size; // 内存大小
	uint32 m_use_size; // 未使用内存
	char* m_begin_mem; // 内存起始指针 (将内存理解成一个圆形结构，循环使用)
	char* m_end_mem; // 内存结束指针
	char* m_begin_data; // 数据起始位置（从前往后移动，到末尾，再从头开始）
	char* m_end_data; // 数据结束位置 （数据下一个位置）
};

BSBuffer operator+(BSBuffer& buffer, BSBuffer& buffer1)
{
	BSBuffer tmpBuffer;
	void* data = malloc(buffer.useSize());
	buffer.takeData(data, buffer.useSize());
	
	void* data1 = malloc(buffer1.useSize());
	buffer1.takeData(data1, buffer1.useSize());

	tmpBuffer.fillData(data, buffer.useSize());
	tmpBuffer.fillData(data1, buffer1.useSize());

	return tmpBuffer;
}

}
using namespace basis;
#endif