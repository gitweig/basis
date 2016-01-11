#include "basis_buffer.h"

namespace basis
{
//////////////////////////////////////////////////////////////////////////
// 自定义缓冲区
// class BSBuffer
//////////////////////////////////////////////////////////////////////////
BSBuffer::BSBuffer()
	: m_size(INIT_MEM)
	, m_use_size(0)
{
	m_begin_mem = (char*)malloc(INIT_MEM);	
	m_end_mem = m_begin_mem + m_size;
	m_begin_data = m_begin_mem;
	m_end_data = m_begin_data;
}

void BSBuffer::reserve(uint32 _size)
{
	if (_size > m_size)
	{
		if (_size > MAX_MEM) return;

		char* _buff = (char*)malloc(_size);
		if (NULL == _buff) return;

		if (m_use_size)
		{
			// 长型结构保存
			if (m_end_data > m_begin_data)
			{
				memmove(_buff, m_begin_data, m_use_size);
			}
			// 环形结构保存 (将环型结构转换成长型结构)
			else
			{
				uint32 after_size =  m_end_mem - m_begin_data;
				uint32 before_size = m_end_data - m_begin_mem;
				memmove(_buff, m_begin_data, after_size);
				memmove(_buff + after_size, m_begin_mem, before_size);
			}
		}

		delete m_begin_mem;
		m_begin_mem = _buff;
		m_end_mem = m_begin_mem + _size;
		m_begin_data = m_begin_mem;
		m_end_data = m_begin_data + m_use_size;
		m_size = _size;
	}
}

bool BSBuffer::fill_data(void* data, uint32 _size)
{
	char* pData = (char*)data;

	// 内存不足，开辟新的内存
	if ((m_size - m_use_size) < _size)
	{
		while ((m_size - m_use_size) < _size)
		{
			m_size *= 2; // 在原来内存基础上扩展一倍
			if (m_size > MAX_MEM)
			{
				return false;
			}
		}

		// 复制原来的内容到新的内存中
		char* tmp = (char*)malloc(m_size);
		
		// 长型内存保存
		if (m_end_data > m_begin_data)
		{
			memmove(tmp, m_begin_data, m_use_size);
		}
		// 环形内存保存
		else
		{
			uint32 after_size = m_end_mem - m_begin_data;
			uint32 before_size = m_end_data - m_begin_mem;
			memmove(tmp, m_begin_data, after_size);
			memmove(tmp + after_size, m_begin_mem, before_size);

			delete m_begin_mem;
			m_begin_mem = tmp;
			m_end_mem = m_begin_mem + m_size;
			m_begin_data = m_begin_mem;
			m_end_data = m_begin_data + m_use_size;
		}
	}

	// 长型内存结构
	if (m_end_data >= m_begin_data)
	{
		// 长型存储
		if ((uint32)(m_end_mem - m_end_data) >= _size)
		{
			memmove(m_end_data, pData, _size);
			m_end_data += _size;
		} 
		// 环型存储
		else
		{
			uint32 mem_size = m_end_mem - m_end_data;
			memmove(m_end_data, pData, mem_size);
			uint32 mem_size1 = _size - mem_size;
			memmove(m_begin_mem, pData + mem_size, mem_size1);
			m_end_data = m_begin_mem + mem_size1;
			//ASSERT(m_end_data < m_begin_data);
		}
	}
	// 环形内存结构
	else if (m_end_data < m_begin_data)
	{
		uint32 free_size = m_begin_data - m_end_data;
		if (_size > free_size)
		{
			return false;
		}
		memmove(m_end_data, pData, _size);
		m_end_data += _size;
	}

	m_use_size += _size;

	return true;
}

bool BSBuffer::take_data(void* data, uint32 _size)
{
	// 这里没有动态释放内存，避免反复开辟和释放内存
	char* pData = (char*)data;

	if (_size > m_size || _size > m_use_size)
	{
		return false;
	}

	// 长型内存
	if (m_end_data > m_begin_data)
	{
		if (m_use_size < _size)
		{
			return false;
		}
		memmove(pData, m_begin_data, _size);
		m_begin_data += _size;
	}
	// 环形内存
	else
	{
		// 向后内存检测
		if ((uint32)(m_end_mem - m_begin_data) >= _size)
		{
			memmove(pData, m_begin_data, _size);
			m_begin_data += _size;
		}
		else
		{
			uint32 mem_size = m_end_mem - m_begin_data;
			// 前部分内存不足
			if ((uint32)(m_end_data - m_begin_mem) < _size - mem_size)
			{
				return false;
			}

			memmove(pData, m_begin_data, mem_size);
			memmove(pData + mem_size, m_begin_mem, _size - mem_size);
			m_begin_data = m_begin_mem + (_size - mem_size);
		}
	}

	m_use_size -= _size;

	return true;
}

BSBuffer::~BSBuffer()
{
	delete m_begin_mem;
	m_begin_mem = NULL;
	m_end_mem = NULL;
	m_begin_data = NULL;
	m_end_data = NULL;
}

BSBuffer operator+(BSBuffer& buffer, BSBuffer& buffer1)
{
	BSBuffer tmpBuffer;
	void* data = malloc(buffer.use_size());
	buffer.take_data(data, buffer.use_size());

	void* data1 = malloc(buffer1.use_size());
	buffer1.take_data(data1, buffer1.use_size());

	tmpBuffer.fill_data(data, buffer.use_size());
	tmpBuffer.fill_data(data1, buffer1.use_size());

	return tmpBuffer;
}

}