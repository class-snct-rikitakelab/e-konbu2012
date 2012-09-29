#ifndef __Gradient
#define __Gradient

#include "../NXTHardWare/GyroSensor.h"
#include "../Common/Common.h"
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
extern BOOL Gradient_dectTargGradient(Gradient *this_Gradient);


#endif


