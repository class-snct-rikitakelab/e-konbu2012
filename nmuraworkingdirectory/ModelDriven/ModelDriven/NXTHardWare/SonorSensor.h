#ifndef __SonorSensor
#define __SonorSensor

#include "ecrobot_interface.h"
/**
 * �����g�Z���T
 */
	/**
	 * �v������
	 */
typedef struct {
S32 port;
}SonorSensor;

void SonorSensor_init(SonorSensor *this_SonorSensor,S32 port);
S32 SonorSensor_getSonor(SonorSensor *this_SonorSensor);

#endif

