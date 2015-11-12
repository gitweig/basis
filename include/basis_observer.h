#ifndef _BASIS_OBSERVER_H_
#define _BASIS_OBSERVER_H_


#include "basis_notification.h"


namespace basis {


	class BSObserver
	{
	public:
		BSObserver() {}
		BSObserver(const BSObserver& observer) {}
		virtual ~BSObserver() {}

		BSObserver& operator = (const BSObserver& observer);

		virtual void notify(BSNotification* pnotification) const = 0;
		virtual bool equals(const BSObserver* pobserver) const = 0;
		virtual bool accepts(BSNotification* pnotification) const = 0;
		virtual BSObserver* clone() const = 0;
		virtual void disable() = 0;
	};


} // namespace basis


#endif // _BASIS_OBSERVER_H_

