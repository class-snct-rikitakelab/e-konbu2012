#ifndef _CURVATURE_ENCODER_H
#define _CURVATURE_ENCODER_H

typedef struct{
}CurvatureEncoder;

extern void CurvatureEncoder_init(CurvatureEncoder *this_CurvatureEncoder);
extern float CurvatureEncoder_getCurvature(CurvatureEncoder *this_CurvatureEncoder);

#endif
