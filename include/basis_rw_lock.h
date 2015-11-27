#ifndef _BASIS_RW_LOCK_H_
#define _BASIS_RW_LOCK_H_

// 读写锁（非递归）
namespace basis
{
template<class T>
class SmartLocker
{
public:
	explicit SmartLocker(const T& lock)
		: m_lock(lock)
	{
		lock.lock();
	}

	unlock()
	{
		m_lock.unlock();
	}

	~SmartLocker()
	{
		m_lock.unlock;
	}

private:
	T& m_lock;
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
	~BSRWLock() {}
	void readLock();
	void writeLock();
	bool tryReadLock();
	bool tryWriteLock();

private:
	class SysRWLock;
	SysRWLock* m_lock;
	int m_pro; // 优先级
};

}
using namespace basis;
#endif