#ifndef __GyroSensor
#define __GyroSensor



#include "ecrobot_interface.h"


/**
 * ジャイロセンサクラス
 */


typedef struct {

}GyroSensor;

void GyroSensor_init(GyroSensor *this_GyroSensor);
	/**
	 * 角速度を取得する
	 */

U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor);

#endif

