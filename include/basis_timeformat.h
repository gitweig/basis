#ifndef _BASIS_TIME_FORMAT_H_
#define _BASIS_TIME_FORMAT_H_

#include "basis_timestamp.h"

namespace basis
{
class BSTimeFormat
{
public:
	BSTimeFormat();
	BSTimeFormat(time_t& timestamp);
	BSTimeFormat(BSTimeStamp& timestamp);

private:
	BSTimeStamp timestamp;

};

}

using namespace basis;
#endif