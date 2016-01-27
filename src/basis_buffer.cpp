#include "basis_buffer.h"

namespace basis
{
//////////////////////////////////////////////////////////////////////////
// �Զ��建����
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
			// ���ͽṹ����
			if (m_end_data > m_begin_data)
			{
				memmove(_buff, m_begin_data, m_use_size);
			}
			// ���νṹ���� (�����ͽṹת���ɳ��ͽṹ)
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

	// �ڴ治�㣬�����µ��ڴ�
	if ((m_size - m_use_size) < _size)
	{
		uint32 buff_size = m_size;
		while ((buff_size - m_use_size) < _size)
		{
			buff_size *= 2; // ��ԭ���ڴ��������չһ��
			if (buff_size > MAX_MEM)
			{
				return false;
			}
		}

		// ���¿����ڴ�
		reserve(buff_size);
	}

	// �����ڴ�ṹ
	if (m_end_data >= m_begin_data)
	{
		// ���ʹ洢
		if ((uint32)(m_end_mem - m_end_data) >= _size)
		{
			memmove(m_end_data, pData, _size);
			m_end_data += _size;
		} 
		// ���ʹ洢
		else
		{
			uint32 mem_size = m_end_mem - m_end_data;
			memmove(m_end_data, pData, mem_size);
			uint32 mem_size1 = _size - mem_size;
			memmove(m_begin_mem, pData + mem_size, mem_size1);
			m_end_data = m_begin_mem + mem_size1;
		}
	}
	// �����ڴ�ṹ
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

	// ����û�ж�̬�ͷ��ڴ棬���ⷴ�����ٺ��ͷ��ڴ�
	char* pData = (char*)data;

	uint32 take_size = _size;

	if (m_use_size < _size)
	{
		take_size = m_use_size;
	}

	// �����ڴ�
	if (m_end_data > m_begin_data)
	{
		memmove(pData, m_begin_data, take_size);
		m_begin_data += take_size;
	}
	// �����ڴ�
	else
	{
		// ����ڴ���
		if ((uint32)(m_end_mem - m_begin_data) >= take_size)
		{
			memmove(pData, m_begin_data, take_size);
			m_begin_data += take_size;
		}
		else
		{
			uint32 mem_size = m_end_mem - m_begin_data;
			// ǰ�����ڴ治��
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
			// ���ͽṹ����
			if (_buff.m_end_data > _buff.m_begin_data)
			{
				memmove(pBuff, _buff.m_begin_data, _buff.m_use_size);
			}
			// ���νṹ���� (�����ͽṹת���ɳ��ͽṹ)
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

}