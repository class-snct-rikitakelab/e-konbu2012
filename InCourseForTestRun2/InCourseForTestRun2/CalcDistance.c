#include "CalcDistance.h"

int getDistance()
{
	int revR,revL;
	revR = nxt_motor_get_count(NXT_PORT_B);
	revL = nxt_motor_get_count(NXT_PORT_C);

	return fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
}
