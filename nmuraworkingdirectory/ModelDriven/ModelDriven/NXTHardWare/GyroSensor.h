#ifndef __GyroSensor
#define __GyroSensor



#include "ecrobot_interface.h"


/**
 * �W���C���Z���T�N���X
 */


typedef struct {

}GyroSensor;

void GyroSensor_init(GyroSensor *this_GyroSensor);
	/**
	 * �p���x���擾����
	 */

U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor);

#endif

