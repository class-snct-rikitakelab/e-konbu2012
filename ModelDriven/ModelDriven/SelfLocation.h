#ifndef __SELFLOCATION
#define __SELFLOCATION

#include "ecrobot_interface.h"
#include "math.h"

typedef struct{
	float distance;		//�ړ�����
	float xCoo;			//x���W
	float yCoo;			//y���W
	float direction;	//����
}SelfLocation;

/* �ԗ֔��a�A���s�̕�*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

#define CIRCUMFERENCE	25.8	//�ԗւ̉~��

extern void SelfLocation_init(SelfLocation * this_SelfLocation);
extern void SelfLocation_SelfLocate(SelfLocation * this_SelfLocation);
extern float SelfLocation_GetDistance(SelfLocation * this_SelfLocation);
extern float SelfLocation_GetXCoo(SelfLocation * this_SelfLocation);
extern float SelfLocation_GetYCoo(SelfLocation * this_SelfLocation);
extern float SelfLocation_GeyDirection(SelfLocation * this_SelfLocation);

#endif
