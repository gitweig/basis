#ifndef _BASIS_RW_LOCK_H_
#define _BASIS_RW_LOCK_H_

#include "basis_define.h"

// 读写锁（非递归）
namespace basis
{
	template<class T, bool isLock = false>
	class SmartLocker
	{
	public:
		explicit SmartLocker(T& lock)
			: m_lock(lock)
			, m_locked(true)
		{
			if (!isLock) lock.lock();
		}

		void unlock()
		{
			if (m_locked)
			{
				m_lock.unlock();
				m_locked = false;
			}
		}

		void lock()
		{
			if (!m_locked)
			{
				m_lock.lock();
				m_locked = true;
			}
		}

		~SmartLocker()
		{
			unlock();
		}

	private:
		T& m_lock;
		bool m_locked;
	};

	//////////////////////////////////////////////////////////////////////////
	class BSRWLock
	{
		enum LOCK_PRO
		{
			READ_PRO,
			WRITE_PRO
		};
	public:
		BSRWLock(int type = WRITE_PRO);
		~BSRWLock();
		void readLock();
		void writeLock();
		void unlockRead();
		void unlockWrite();
		void unlock();
		bool tryReadLock(uint32 _sec);
		bool tryWriteLock(uint32 _sec);
	private:
		BSRWLock(const BSRWLock& rw);
		BSRWLock& operator=(const BSRWLock&);

	private:
		class SysRWLock;
		SysRWLock* m_lock;
		int m_pro; // 优先级
	};

}
using namespace basis;
#endif