#ifndef TYREAL_TYREAL_H
#define TYREAL_TYREAL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "tyreal/ADJUST_PARAM_STATE.h"

namespace tyreal
{
class tyreal
{
private:
	int forward_val;
	int turn_val;
	int tail_angle;
	float Kp;
	float Ki;
	float Kd;
	char[INT_EXPRESS_RENGE] float_string;
	char[100] print_string;
	char[100] int_string;
public:
	ADJUST_PARAM_STATE adjust_param_state;
 
public:
	void do_tyreal();
	float change_float_param();
	int change_int_param();
};

}  // namespace tyreal
#endif
