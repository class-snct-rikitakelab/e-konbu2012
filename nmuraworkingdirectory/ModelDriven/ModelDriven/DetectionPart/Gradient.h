#ifndef __Gradient
#define __Gradient

#include "../NXTHardWare/GyroSensor.h"
#include "../Common/Common.h"
/**
 * 傾き検出
 */
typedef struct {
	/**
	 * 指定角速度
	 */
	U16 TargetGradient;
}
Gradient;

void Gradient_init(Gradient *this_Gradient);

	/**
	 * 指定角速度を検知する
	 */
extern BOOL Gradient_dectTargGradient(Gradient *this_Gradient);


#endif


