#ifndef __Gradient
#define __Gradient

#include "../DetectionPart/Notice.h"
#include "../NXTHardWare/GyroSensor.h"

/**
 * �X�����o
 */
typedef struct {
	/**
	 * �w��p���x
	 */
	U16 TargetGradient;
}
Gradient;

void Gradient_init(Gradient *this_Gradient);

	/**
	 * �w��p���x�����m����
	 */
BOOL Gradient_dectTargGradient(Gradient *this_Gradient);


#endif


