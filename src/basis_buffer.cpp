#include "basis_buffer.h"

namespace basis
{
//////////////////////////////////////////////////////////////////////////
// 自定义缓冲区
// class BSBuffer
//////////////////////////////////////////////////////////////////////////
BSBuffer::BSBuffer()
	: m_size(0)
	, m_use_size(0)
	, m_begin_mem(NULL)
	, m_end_mem(NULL)
	, m_begin_data(NULL)
	, m_end_data(NULL)
{
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

		free(m_begin_mem);
		m_begin_mem = _buff;
		m_end_mem = m_begin_mem + _size;
		m_begin_data = m_begin_mem;
		m_end_data = m_begin_data + m_use_size;
		m_size = _size;
	}
}

bool BSBuffer::fill_data(const void* data, uint32 _size)
{
	if (NULL == data) return false;

	const char* pData = (const char*)data;

	// 内存不足，开辟新的内存
	if ((m_size - m_use_size) < _size)
	{
		uint32 buff_size = m_size;
		while ((buff_size - m_use_size) < _size)
		{
			buff_size *= 2; // 在原来内存基础上扩展一倍
			if (buff_size > MAX_MEM)
			{
				return false;
			}
		}

		// 重新开辟内存
		reserve(buff_size);
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

uint32 BSBuffer::take_data(void* data, uint32 _size)
{
	if (NULL == data) return 0;

	// 这里没有动态释放内存，避免反复开辟和释放内存
	char* pData = (char*)data;

	uint32 take_size = _size;

	if (m_use_size < _size)
	{
		take_size = m_use_size;
	}

	// 长型内存
	if (m_end_data > m_begin_data)
	{
		memmove(pData, m_begin_data, take_size);
		m_begin_data += take_size;
	}
	// 环形内存
	else
	{
		// 向后内存检测
		if ((uint32)(m_end_mem - m_begin_data) >= take_size)
		{
			memmove(pData, m_begin_data, take_size);
			m_begin_data += take_size;
		}
		else
		{
			uint32 mem_size = m_end_mem - m_begin_data;
			// 前部分内存不足
			if ((uint32)(m_end_data - m_begin_mem) < take_size - mem_size)
			{
				return false;
			}

			memmove(pData, m_begin_data, mem_size);
			memmove(pData + mem_size, m_begin_mem, take_size - mem_size);
			m_begin_data = m_begin_mem + (take_size - mem_size);
		}
	}

	m_use_size -= take_size;

	return take_size;
}

BSBuffer& BSBuffer::operator=(const BSBuffer& _buff)
{
	char* pBuff = (char*)malloc(_buff.capatiy());

	if (pBuff)
	{
		if (_buff.m_use_size)
		{
			// 长型结构保存
			if (_buff.m_end_data > _buff.m_begin_data)
			{
				memmove(pBuff, _buff.m_begin_data, _buff.m_use_size);
			}
			// 环形结构保存 (将环型结构转换成长型结构)
			else
			{
				uint32 after_size =  m_end_mem - m_begin_data;
				uint32 before_size = m_end_data - m_begin_mem;
				memmove(pBuff, _buff.m_begin_data, after_size);
				memmove(pBuff + after_size, _buff.m_begin_mem, before_size);
			}
		}

		m_begin_mem = pBuff;
		m_end_mem = m_begin_mem + _buff.m_use_size;
		m_begin_data = m_begin_mem;
		m_end_data = m_begin_data + _buff.m_use_size;
		m_size = _buff.m_size;
	}

	return *this;
}

BSBuffer::BSBuffer(const BSBuffer& _buff)
{
	if (NULL != m_begin_mem)
	{
		free(m_begin_mem);
		m_begin_mem = NULL;
	}

	*this = _buff;
}

BSBuffer::~BSBuffer()
{
	free(m_begin_mem);
	m_begin_mem = NULL;
	m_end_mem = NULL;
	m_begin_data = NULL;
	m_end_data = NULL;
}

BSBuffer operator+(BSBuffer& buffer, BSBuffer& buffer1)
{
	BSBuffer tmpBuffer;
	void* data = malloc(buffer.use_size());
	if (NULL == data) return BSBuffer();

	if (buffer.use_size() != buffer.take_data(data, buffer.use_size()))
	{
		return BSBuffer();
	}

	void* data1 = malloc(buffer1.use_size());
	if (NULL == data1) return BSBuffer();
	if (buffer1.use_size() != buffer1.take_data(data1, buffer1.use_size()))
	{
		return BSBuffer();
	}

	tmpBuffer.fill_data(data, buffer.use_size());
	tmpBuffer.fill_data(data1, buffer1.use_size());

	return tmpBuffer;
}

}