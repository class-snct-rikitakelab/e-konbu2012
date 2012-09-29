
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace NXTHardWare
{
class MotorEncoder
{
public:
	void MotorEncoder_init(MotorEncoder *this_MotorEncoder);

	int MotorEncoder_getCount(MotorEncoder *this_MotorEncoder);

};

}  // namespace NXTHardWare
