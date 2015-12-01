#include "basis_rw_lock.h"
#include "basis_event.h"
#include "basis_mutex.h"
#include "basis_critical.h"
#include "basis_scope_locker.h"
#include "basis_macro.h"
#include "basis_thread.h"

namespace basis
{
#ifdef __WINDOWS__
class BSRWLock::SysRWLock
{	
public:
	SysRWLock(int _type)
		: m_type(_type)
		, m_readCount(0)
		, m_writeCount(0)
		, m_rWait(0)
		, m_wWait(0)
		, m_signal(true) // 人为控制信号
	{
	}

	~SysRWLock()
	{
		ASSERT((!m_rWait && !m_wWait));
		ASSERTMSG(!readTotal(), "the rw locker is currently locked");
		ASSERTMSG(!m_writeThread, "the rw locker is currently locked");
	}

	void readLock()
	{
		m_locker.lock();
		ASSERTMSG(thread_id() != m_writeThread, "have write lock, can't read lock");
		ASSERTMSG(!readCount(thread_id()), "rw locker can't recurrence lock");

		// 现在是写状态 或者是有写等待
		while (m_writeCount || m_wWait)
		{
			++m_rWait;
			m_signal.reset(); // 设置为无信号状态,为了后续等待
			m_locker.unlock();
			m_signal.wait(); // 等待获取资源
			m_locker.lock();
			--m_rWait;
			//locker.unlock();
		}

		//locker.lock();
		readInsert(thread_id());
		++m_readCount; 
		m_locker.unlock();

	}

	bool tryReadLock(uint32 _sec)
	{
		m_locker.lock();
		if (WRITE_PRO == m_type && m_wWait) return false;

		ASSERTMSG(basis::thread_id() != m_writeThread, "have write lock, can't read lock");
		ASSERTMSG(!readCount(basis::thread_id()), "rw locker can't recurrence lock");

		// 检测写状态 或者是 写等待
		if (m_writeCount || m_wWait)
		{
			m_locker.unlock(); // 放开m_isWrite状态，防止死锁

			m_signal.wait(_sec);

			m_locker.lock();
			// 检测现在状态
			if (m_writeCount) 
			{
				m_locker.unlock();
				return false;
			}
			else 
			{
				++m_readCount;
				m_locker.unlock();
			}
		}

		return true;
	}

	void unlockRead()
	{
		m_locker.lock();

		if (m_readCount > 0)
		{
			--m_readCount;
			ASSERT((readErase(thread_id()), true));
			if (!m_readCount) 
			{
				if (m_wWait || m_rWait)
				{
					m_signal.set();
				}
			}
		}
		m_locker.unlock();
	}

	void writeLock()
	{	
		m_locker.lock();

		ASSERTMSG(basis::thread_id() != m_writeThread, "have write lock, can't read lock");
		ASSERTMSG(!readCount(basis::thread_id()), "rw locker can't recurrence lock");

		while (m_readCount || m_writeCount)
		{
			++m_wWait; // 增加写等待
			m_signal.reset(); // 为了防止信号的混乱，这里重置为无信号状态
			m_locker.unlock();
			m_signal.wait(); // 等待信号
			m_locker.lock();
			--m_wWait;
			//locker.unlock();
		}

		// 空闲状态
		//locker.lock();
		setWriteThreadId(thread_id());
		++m_writeCount;
		m_locker.unlock();
	}

	bool tryWriteLock(uint32 _sec = 0)
	{
		m_locker.lock();

		ASSERTMSG(thread_id() != m_writeThread, "have write lock, can't read lock");
		ASSERTMSG(!readCount(thread_id()), "rw locker can't recurrence lock");

		// 检测读状态
		if (m_readCount || m_writeCount)
		{
			m_locker.unlock();
			m_signal.wait(_sec);
			m_locker.lock();
			// 检测是否超时（防止意外情况，没有使用wait返回值）
			if (m_readCount || m_writeCount)
			{
				m_locker.unlock();
				return false;
			}
			else
			{
				++m_writeCount;
				m_locker.unlock();
			}
		}

		return true;
	}

	void unlockWrite()
	{
		m_locker.lock();
		if (m_writeCount)
		{
			--m_writeCount;
			if (!m_writeCount)
			{
				// 写线程置零
				setWriteThreadId(0);
				if (m_wWait > 0 || m_rWait > 0)
				{
					m_signal.set();
				}
			}
		}	

		m_locker.unlock();
	}

private:
	int m_type; // 读写优先
	BSCritical m_locker; // 整个数据的锁
	uint32 m_readCount; // 读数量
	uint32 m_writeCount; // 写数量
	uint32 m_rWait; // 读等待数量
	uint32 m_wWait; // 写等待数量
	BSEvent m_signal; //  读写信号

#ifdef __DEBUG__
	uint32 m_writeThread; // 写线程id
	set<uint32> m_readThreadList; // 读线程列表
	BSCritical m_thread_lock;

	void setWriteThreadId(uint32 threadId)
	{
		m_writeThread = threadId;
	}

	uint32 readCount(uint32 id)
	{
		BSScopeLocker<BSCritical> lock_(m_thread_lock);
		return (uint32)m_readThreadList.count(id);
	}
	uint32 readTotal()
	{
		BSScopeLocker<BSCritical> lock_(m_thread_lock);
		return (uint32)m_readThreadList.size();
	}
	void readInsert(uint32 id)
	{
		BSScopeLocker<BSCritical> lock_(m_thread_lock);
		m_readThreadList.insert(id);
	}
	void readErase(uint32 id)
	{
		BSScopeLocker<BSCritical> lock_(m_thread_lock);
		m_readThreadList.erase(id);
	}

#endif
};

#endif

#ifdef __POSIX__
class BSRWLock::SysRWLock
{
public:
	SysRWLock(int _type)
	{
		pthread_rwlockattr_t rwAttr;
		pthread_rwlockattr_init(&rwAttr);
		if (READ_PRO == _type)
		{
			VERIFY(!pthread_rwlockattr_setkind_np(&rwAttr, PTHREAD_RWLOCK_PREFER_READER_NP));
		}
		else if (WRITE_PRO == _type)
		{  // 必须是PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP， 才能写锁优先
			// PTHREAD_RWLOCK_PREFER_WRITER_NP 并不能达到写锁优先的效果
			VERIFY(!pthread_rwlockattr_setkind_np(&rwAttr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP));
		}
		else
		{
			VERIFY(false)
		}

		VERIFY(!pthread_rwlock_init(&m_rwl, &rwAttr));
		VERIFY(!pthread_rwlockattr_destroy(&rwAttr));
	}

	~SysRWLock()
	{
		VERIFY(!pthread_rwlock_destroy(&m_rwLock));
	}

	void readLock()
	{
		VERIFY(pthread_rwlock_rdlock(&m_rwLock));
	}

	void writeLock()
	{
		VERIFY(pthread_rwlock_wrlock(&m_rwLock));
	}

	bool tryReadLock()
	{
		return 0 == pthread_rwlock_tryrdlock(&m_rwLock);
	}

	bool tryWriteLock()
	{
		return 0 == pthread_rwlock_trywrlock(&m_rwLock);
	}

private:
	pthread_rwlock_t m_rwLock;
};
#endif

BSRWLock::BSRWLock(int type)
	: m_lock(new SysRWLock(WRITE_PRO))
	, m_type(type)
{

}

BSRWLock::~BSRWLock()
{
	if (NULL != m_lock)
	{
		delete m_lock;
	}
}

void BSRWLock::readLock()
{
	if (NULL != m_lock)
	{
		m_lock->readLock();
	}
}

bool BSRWLock::tryReadLock(uint32 _sec)
{
	if (NULL != m_lock)
	{
		return m_lock->tryReadLock(_sec);
	}

	return false;
}

bool BSRWLock::tryWriteLock(uint32 _sec)
{
	if (NULL != m_lock)
	{
		return m_lock->tryReadLock(_sec);
	}

	return false;
}

void BSRWLock::writeLock()
{
	if (NULL != m_lock)
	{
		m_lock->writeLock();
	}
}

void BSRWLock::unlockRead()
{
	if (NULL != m_lock)
	{
		m_lock->unlockRead();
	}
}

void BSRWLock::unlockWrite()
{
	if (NULL != m_lock)
	{
		m_lock->unlockWrite();
	}
}


}