#ifndef _CURVATURE_H
#define _CURVATURE_H

#include "../DrivenPart/CurvatureEncoder.h"

typedef struct{
	float targCurvature;	//–Ú•W‹È—¦”¼Œa
}Curvature;

extern float Curvature_getCurvature(Curvature * this_Curvature);
extern float Curvature_getTargCurvature(Curvature * this_Curvature);
extern void Curvature_setTargCurvature(Curvature * this_Curvature,float targCurvature);
extern void Curvature_init(Curvature * this_Curvature);

#endif
