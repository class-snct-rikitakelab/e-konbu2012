#include "Curvature.h"

float Curvature_getCurvature(Curvature * this_Curvature)
{
	return 0;
}

float Curvature_getTargCurvarture(Curvature * this_Curvature)
{
	return this_Curvature->targCurvature;
}

void Curvature_setTargCurvature(Curvature * this_Curvature,float targCurvature)
{
	this_Curvature->targCurvature = targCurvature;
}
