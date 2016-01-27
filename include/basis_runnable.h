#ifndef _BASIS_RUNNABLE_H_
#define _BASIS_RUNNABLE_H_

#include "basis_define.h"
#include "basis_smart_ptr.h"

namespace basis
{
class BSThread;

class BSRunnable
{
public:
	virtual void run(BSThread* pthread) = 0;
};

typedef shared_ptr<BSRunnable> BSRunnablePtr;

} //namespace basis

#endif //_BASIS_RUNNABLE_H_

