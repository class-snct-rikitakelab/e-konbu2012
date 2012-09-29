#ifndef __SELFLOCATION
#define __SELFLOCATION

#include "ecrobot_interface.h"
#include "math.h"

typedef struct{
	float distance;		//移動距離
	float xCoo;			//x座標
	float yCoo;			//y座標
	float direction;	//方向
}SelfLocation;

/* 車輪半径、走行体幅*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

#define CIRCUMFERENCE	25.8	//車輪の円周

extern void SelfLocation_init(SelfLocation * this_SelfLocation);
extern void SelfLocation_SelfLocate(SelfLocation * this_SelfLocation);
extern float SelfLocation_GetDistance(SelfLocation * this_SelfLocation);
extern float SelfLocation_GetXCoo(SelfLocation * this_SelfLocation);
extern float SelfLocation_GetYCoo(SelfLocation * this_SelfLocation);
extern float SelfLocation_GeyDirection(SelfLocation * this_SelfLocation);

#endif
