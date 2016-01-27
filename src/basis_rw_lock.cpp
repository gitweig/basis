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
		, m_signal(true) // ��Ϊ�����ź�
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

		// ������д״̬ ��������д�ȴ�
		while (m_writeCount || m_wWait)
		{
			++m_rWait;
			m_signal.reset(); // ����Ϊ���ź�״̬,Ϊ�˺����ȴ�
			m_locker.unlock();
			m_signal.wait(); // �ȴ���ȡ��Դ
			m_locker.lock();
			--m_rWait;
		}

		ASSERT((readInsert(thread_id()), true));
		++m_readCount; 
		m_locker.unlock();
	}

	bool tryReadLock(uint32 _sec)
	{
		m_locker.lock();

		ASSERTMSG(basis::thread_id() != m_writeThread, "have write lock, can't read lock");
		ASSERTMSG(!readCount(basis::thread_id()), "rw locker can't recurrence lock");

		// ���д״̬ ������ д�ȴ�
		if (m_writeCount || m_wWait)
		{
			++m_rWait;
			m_locker.unlock();
			m_signal.wait(_sec);
			m_locker.lock();
			--m_rWait;
			// �������״̬

			if (m_writeCount || m_wWait) 
			{
				m_locker.unlock();
				return false;
			}
		}

		++m_readCount;
		m_locker.unlock();
		return true;
	}

	void unlockRead()
	{
		m_locker.lock();

		ASSERT(m_readCount > 0);
		if (m_readCount > 0)
		{
			ASSERT((readErase(thread_id()), true));
			if (!m_readCount--) 
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

		ASSERTMSG(basis::thread_id() != m_writeThread, "have write lock, can't recurrence lock");
		ASSERTMSG(!readCount(basis::thread_id()), "rw locker can't recurrence lock");

		while (m_readCount || m_writeCount)
		{
			++m_wWait; // ����д�ȴ�
			m_signal.reset(); // Ϊ�˷�ֹ�źŵĻ��ң���������Ϊ���ź�״̬
			m_locker.unlock();
			m_signal.wait(); // �ȴ��ź�
			m_locker.lock();
			--m_wWait;
		}

		// ����״̬;
		ASSERT((setWriteThreadId(thread_id()), true));
		++m_writeCount;
		m_locker.unlock();
	}

	bool tryWriteLock(uint32 _sec = 0)
	{
		m_locker.lock();

		ASSERTMSG(thread_id() != m_writeThread, "have write lock, can't recurrence lock");
		ASSERTMSG(!readCount(thread_id()), "rw locker can't recurrence lock");

		// ����״̬
		if (m_readCount || m_writeCount)
		{
			++m_wWait;
			m_locker.unlock();
			m_signal.wait(_sec);
			m_locker.lock();
			--m_wWait;
			// ����Ƿ�ʱ����ֹ���������û��ʹ��wait����ֵ��
			if (m_readCount || m_writeCount)
			{
				m_locker.unlock();
				return false;
			}
		}

		++m_writeCount;
		m_locker.unlock();

		return true;
	}

	void unlockWrite()
	{
		m_locker.lock();

		ASSERT(m_writeCount > 0);
		if (m_writeCount > 0)
		{
			if (!m_writeCount--)
			{
				// д�߳�����
				ASSERT((setWriteThreadId(0), true));
				if (m_wWait > 0 || m_rWait > 0)
				{
					m_signal.set();
				}
			}
		}	

		m_locker.unlock();
	}

private:
	int m_type; // ��д����
	BSCritical m_locker; // �������ݵ���
	uint32 m_readCount; // ������
	uint32 m_writeCount; // д����
	uint32 m_rWait; // ���ȴ�����
	uint32 m_wWait; // д�ȴ�����
	BSEvent m_signal; //  ��д�ź�

#ifdef __DEBUG__
	uint32 m_writeThread; // д�߳�id
	set<uint32> m_readThreadList; // ���߳��б�
	
	void setWriteThreadId(uint32 threadId)
	{
		m_writeThread = threadId;
	}

	uint32 readCount(uint32 id)
	{
		return (uint32)m_readThreadList.count(id);
	}
	uint32 readTotal()
	{
		return (uint32)m_readThreadList.size();
	}
	void readInsert(uint32 id)
	{
		m_readThreadList.insert(id);
	}
	void readErase(uint32 id)
	{
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
		{  // ������PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP�� ����д������
			// PTHREAD_RWLOCK_PREFER_WRITER_NP �����ܴﵽд�����ȵ�Ч��
			VERIFY(!pthread_rwlockattr_setkind_np(&rwAttr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP));
		}
		else
		{
			UNEXPECT();
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

	void unlockRead()
	{
		VERIFY(pthread_rwlock_unlock(&m_rwLock));
	}

	void unlockWrite()
	{
		VERIFY(pthread_rwlock_unlock(&m_rwLock));
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