#ifndef _CURVATURE_H
#define _CURVATURE_H

typedef struct{
	float targCurvature;
}Curvature;

/**
 * ‹È—¦”¼Œa
 */

	/**
	 * ‹È—¦”¼Œa–Ú•W’l
	 */

	/**
	 * ‹È—¦”¼Œa‚ğæ“¾‚·‚é
	 */
	float Curvature_getCurvature(Curvature * this_Curvature);

	/**
	 * ‹È—¦”¼Œa–Ú•W’l‚ğİ’è‚·‚é
	 */
	float Curvature_getTargCurvarture(Curvature * this_Curvature);

	/**
	 * ‹È—¦”¼Œa–Ú•W’l‚ğİ’è‚·‚é
	 */
	void Curvature_setTargCurvature(Curvature * this_Curvature,float targCurvature);

#endif
