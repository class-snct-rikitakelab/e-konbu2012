#ifndef _CURVATURE_H
#define _CURVATURE_H

typedef struct{
	float targCurvature;
}Curvature;

/**
 * �ȗ����a
 */

	/**
	 * �ȗ����a�ڕW�l
	 */

	/**
	 * �ȗ����a���擾����
	 */
	float Curvature_getCurvature(Curvature * this_Curvature);

	/**
	 * �ȗ����a�ڕW�l��ݒ肷��
	 */
	float Curvature_getTargCurvarture(Curvature * this_Curvature);

	/**
	 * �ȗ����a�ڕW�l��ݒ肷��
	 */
	void Curvature_setTargCurvature(Curvature * this_Curvature,float targCurvature);

#endif
