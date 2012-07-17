#ifndef TYREAL_A_D_J_U_S_T__P_A_R_A_M__S_T_A_T_E_H
#define TYREAL_A_D_J_U_S_T__P_A_R_A_M__S_T_A_T_E_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace tyreal
{
typedef enum 
{
	ADJUST_FORWARD,
	ADJUST_TURN,
	ADJUST_TAIL_ANGLE,
	ADJUST_Kp_VAL,
	ADJUST_Ki_VAL,
	ADJUST_Kd_VAL
} ADJUST_PARAM_STATE;

}  // namespace tyreal
#endif
