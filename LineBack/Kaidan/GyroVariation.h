#ifndef __GyroVariation
#define __GyroVariation

//#include "DetectLineState.h"

typedef struct {

float gyroSensorVarieation;

}GyroVariation;


extern void  GyroVariation_init(GyroVariation * this_GyroVariation);

extern void  GyroVariation_calGyroSensorVariation(GyroVariation * this_GyroVariation);

extern float GyroVariation_getGyroSensorVariation(GyroVariation * this_GyroVariation);

#endif
