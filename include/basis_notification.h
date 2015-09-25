#ifndef _BASIS_NOTIFICATION_H_
#define _BASIS_NOTIFICATION_H_

#include "basis_define.h"

namespace basis {


class BSNotification
{
public:
	BSNotification();
	virtual ~BSNotification();

	virtual std::string name() const;

};


} // namespace basis


#endif // _BASIS_NOTIFICATION_H_
