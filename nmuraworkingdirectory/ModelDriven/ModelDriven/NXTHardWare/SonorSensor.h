#ifndef __SonorSensor
#define __SonorSensor

#include "ecrobot_interface.h"
/**
 * ’´‰¹”gƒZƒ“ƒT
 */
	/**
	 * Œv‘ª‚·‚é
	 */
typedef struct {
S32 port;
}SonorSensor;

void SonorSensor_init(SonorSensor *this_SonorSensor,S32 port);
S32 SonorSensor_getSonor(SonorSensor *this_SonorSensor);

#endif

