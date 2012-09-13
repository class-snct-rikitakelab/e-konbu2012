#ifndef �쓮��_CURVATURE_H
#define �쓮��_CURVATURE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/CurvatureEncoder.h"

namespace �쓮��
{
/**
 * �ȗ����a
 */
class Curvature
{
private:
	/**
	 * �ȗ����a�ڕW�l
	 */
	float targCurvature;

	CurvatureEncoder curvatureEncoder;

public:
	/**
	 * �ȗ����a���擾����
	 */
	float Curvature_getCurvature();

	/**
	 * �ȗ����a�ڕW�l��ݒ肷��
	 */
	float Curvature_getTargCurvarture();

	/**
	 * �ȗ����a�ڕW�l��ݒ肷��
	 */
	void Curvature_setTargCurvature(float targCurvature);

};

}  // namespace �쓮��
#endif
