#include "basis_rw_lock.h"
#include "basis_atomic.h"
#include "basis_mutex.h"
#include "basis_event.h"

namespace basis
{

#ifdef __WINDOWS__
class BSRWLock::SysRWLock
{
	enum {MAX_READ = 1000};
	
public:
	SysRWLock(int _type)
		: m_type(_type)
		, m_readCount(0)
		, m_writeCount(0)
		, m_writeWait(false)
	{
		
	}
	
	~SysRWLock()
	{

	}

	void readLock()
	{
		// 将所有的线程操作，挡在最外边
		SmartLocker<BSMutex> locker(m_allLock);
		// 暂时没考虑锁递归问题
		// 有线程正在写  或者 是有写等待
		// 等待 。。。
		while (m_writeCount || m_writeWait) { sleep(5); }

		// 资源空闲，或者正在进行读进行操作
		if (0 == m_readCount)
		{
			m_signal.wait();
		}

		++m_readCount;
	}

	void writeLock()
	{
		SmartLocker<BSMutex> locker(m_allLock);
		while (true)
		{
			// 没有读和写操作
			if (0 == m_readCount && 0 == m_writeCount)
			{
				++m_writeCount;
				m_signal.wait();
				break;
			}
		}
	}

	bool tryReadLock(uint32 _sec = 0)
	{
		SmartLocker<BSMutex> locker(m_allLock);
		// 有写操作或者写等待时，可以进行读等待
		bool isRead = false;
		if (m_writeCount || !m_writeWait)
		{
			isRead = m_signal.wait(_sec);
		}

		// 如果可以进行读操作
		if (isRead)
		{
			++m_readCount;
		}

		return isRead;
	}

	bool tryWriteLock(uint32 _sec = 0)
	{
		SmartLocker<BSMutex> locker(m_allLock);
		// 有读或者写操作时，进行写等待
		bool tmpRW = false;
		
		if (m_readCount || m_writeWait)
		{
			tmpRW = m_signal.wait(_sec);
		}

		if (tmpRW && (m_writeCount && m_readCount))
		{
			++m_writeCount;
			return true;
		}

		return false;
	}

	void unlockRead()
	{
		SmartLocker<BSMutex> locker(m_allLock);
		VERIFY(0 == m_readCount);

		m_signal.setEvent();
	}

	void unlockWrite()
	{
		SmartLocker<BSMutex> locker(m_allLock);
		VERIFY(0 == m_writeCount);

		m_signal.setEvent();
	}
		
private:
	int m_type; // 读写优先
	BSMutex m_allLock; // 整个数据的锁
	uint32 m_readCount; // 读数量
	uint32 m_writeCount; // 写数量
	BSEvent m_signal; //  读信号
	bool m_writeWait; // 正在写等待
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
{

}

}