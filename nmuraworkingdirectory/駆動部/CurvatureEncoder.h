#ifndef �쓮��_CURVATURE_ENCODER_H
#define �쓮��_CURVATURE_ENCODER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace �쓮��
{
/**
 * �ȗ����a�v����
 * 
 */
class CurvatureEncoder
{
public:
	void CurvatureEncoder_init(CurvatureEncoder *this_CurvatureEncoder);

	/**
	 * �ȗ����a���擾����
	 */
	float CurvatureEncoder_getCurvature(CurvatureEncoder *this_CurvatureEncoder);

};

}  // namespace �쓮��
#endif
