#include "basis_subject.h"


namespace basis {

	BSSubject::BSSubject()
	{

	}

	BSSubject::~BSSubject()
	{
		while (!m_observers.empty())
		{
			delete *m_observers.begin();
		}
	}

	void BSSubject::register_observer( const BSObserver& observer )
	{
		m_observers.push_back(observer.clone());
	}

	void BSSubject::remove_observer( const BSObserver& observer )
	{
		list<BSObserver*>::iterator it = m_observers.begin();
		while (it != m_observers.end())
		{
			if ((*it)->equals(&observer))
			{
				delete *it;
				m_observers.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	bool BSSubject::has_observers() const
	{
		return !m_observers.empty();
	}

	uint32 BSSubject::count_observers() const
	{
		return m_observers.size();
	}

	bool BSSubject::has_observer( const BSObserver& observer ) const
	{
		list<BSObserver*>::const_iterator it = m_observers.begin();
		for (; it != m_observers.end(); ++it)
		{
			if ((*it)->equals(&observer))
			{
				return true;
			}
		}
		return false;
	}

	void BSSubject::notify_observers( BSNotification* pnotification )
	{
		list<BSObserver*>::iterator it = m_observers.begin();
		for (; it != m_observers.end(); ++it)
		{
			(*it)->notify(pnotification);
		}
	}

} // namespace basis
