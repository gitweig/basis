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
	static T m_default;
	
public:
	BSSyncQueue(bool fifo = true);
	~BSSyncQueue();

	uint32 size();

	bool push(const element_type& x);
	
	bool pull(element_type& x);
	bool pull(element_type& x, uint32 ms); // 如果多个线程调用pull 有等待时间不准确的可能

private:
	bool m_fifo;
	BSCritical m_lock;
	BSEvent m_pull_event;
	deque< element_type > m_queue;
};

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
	while (m_queue.empty())
	{
		m_lock.unlock();
		if (!m_pull_event.wait(ms))
		{
			return false;
		}
		m_lock.lock();
	}
	x = m_queue.front();
	m_queue.pop_front();

	m_lock.unlock();
	return false;
}

template<typename T>
bool basis::BSSyncQueue<T>::pull( element_type& x )
{
	m_lock.lock();
	if (m_fifo)
	{
		m_queue.push_back(x);
	}
	else
	{
		m_queue.push_front(x);
	}
	m_pull_event.set();
	m_lock.unlock();
	return true;
}

template<typename T>
bool basis::BSSyncQueue<T>::push( const element_type& x )
{
	return push( x, -1 );
}

template<typename T>
basis::BSSyncQueue<T>::~BSSyncQueue()
{

}

template<typename T>
basis::BSSyncQueue<T>::BSSyncQueue(bool fifo)
	: m_fifo(fifo)
{
}

template<typename T>
T basis::BSSyncQueue<T>::m_default;

}; //namespace basis

#endif //_BASIS_SYNC_QUEUE_H_

