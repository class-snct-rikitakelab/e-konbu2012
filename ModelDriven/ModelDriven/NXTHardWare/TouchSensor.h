#ifndef __TouchSensor
#define __TouchSensor


#include "ecrobot_interface.h"

/**
 * �^�b�`�Z���T
 */
typedef struct {

}TouchSensor;

void TouchSensor_init(TouchSensor *this_TouchSensor);

U8 TouchSensor_getTouchSensor(TouchSensor *this_TouchSensor);

#endif
