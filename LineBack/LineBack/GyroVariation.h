#ifndef __GyroVariation
#define __GyroVariation

//#include "DetectLineState.h"

typedef struct {

S16 gyroSensorVarieation;

}GyroVariation;


extern void  GyroVariation_init(GyroVariation * this_GyroVariation);

extern void  GyroVariation_calGyroSensorVariation(GyroVariation * this_GyroVariation);

extern S16 GyroVariation_getGyroSensorVariation(GyroVariation * this_GyroVariation);

#endif
