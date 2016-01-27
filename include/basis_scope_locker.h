#ifndef _SCOPE_LOCKER_H_
#define _SCOPE_LOCKER_H_

// ÇøÓòËø
namespace basis
{
template<class T, bool isLock = false>
class BSScopeLocker : public BSNoCopy
{
public:
	explicit BSScopeLocker(T& lock)
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

	~BSScopeLocker()
	{
		unlock();
	}

private:
	T& m_lock;
	bool m_locked;
};

}; //namespace basis

#endif //_SCOPE_LOCKER_H_