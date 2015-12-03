#ifndef _BASIS_SYNC_QUEUE_H_
#define _BASIS_SYNC_QUEUE_H_

#include "basis_define.h"
#include "basis_nocopy.h"
#include "basis_scope_locker.h"
#include "basis_critical.h"
#include <deque>
#include "basis_event.h"

namespace basis
{

template<typename T>
class BSSyncQueue : public BSNoCopy
{
	typedef T element_type;
	
public:
	BSSyncQueue(bool fifo = true);
	~BSSyncQueue();

	uint32 size();
	void close(); // 停止服务 唤醒等待线程

	bool push(const element_type& x);
	bool pull(element_type& x, uint32 ms = -1); // 多个线程pull 超时时间有不准确的可能性

private:
	bool m_fifo;
	bool m_exit;
	BSAtomic32 m_wait;
	BSCritical m_lock;
	BSEvent m_pull_event;
	deque< element_type > m_queue;
};

template<typename T>
void basis::BSSyncQueue<T>::close()
{
	m_lock.lock();
	m_exit = true;
	m_lock.unlock();

	for (int32 i = 0; i < m_wait.get_value(); ++i)
	{
		m_pull_event.set();
	}
}

template<typename T>
uint32 basis::BSSyncQueue<T>::size()
{
	BSScopeLocker<BSCritical> locker(m_lock);
	return m_queue.size();
}

template<typename T>
bool basis::BSSyncQueue<T>::pull( element_type& x, uint32 ms )
{
	m_lock.lock();
	if (m_exit)
	{
		m_lock.unlock();
		return false;
	}
	while (m_queue.empty())
	{
		m_lock.unlock();
		m_wait++;
		if (!m_pull_event.wait(ms) || m_exit)
		{
			m_wait--;
			return false;
		}
		m_wait--;
		m_lock.lock();
	}
	x = m_queue.front();
	m_queue.pop_front();

	m_lock.unlock();
	return false;
}

template<typename T>
bool basis::BSSyncQueue<T>::push( const element_type& x )
{
	m_lock.lock();
	if (m_exit)
	{
		m_lock.unlock();
		return false;
	}
	if (m_fifo)
	{
		m_queue.push_back(x);
	}
	else
	{
		m_queue.push_front(x);
	}
	m_lock.unlock();
	m_pull_event.set();
	return true;
}

template<typename T>
basis::BSSyncQueue<T>::~BSSyncQueue()
{
}

template<typename T>
basis::BSSyncQueue<T>::BSSyncQueue(bool fifo)
	: m_fifo(fifo)
	, m_exit(false)
	, m_wait(0)
{
}

}; //namespace basis

#endif //_BASIS_SYNC_QUEUE_H_

