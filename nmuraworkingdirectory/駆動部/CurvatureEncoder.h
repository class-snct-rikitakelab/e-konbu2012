#ifndef ‹ì“®•”_CURVATURE_ENCODER_H
#define ‹ì“®•”_CURVATURE_ENCODER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace ‹ì“®•”
{
/**
 * ‹È—¦”¼ŒaŒv‘ªŠí
 * 
 */
class CurvatureEncoder
{
public:
	void CurvatureEncoder_init(CurvatureEncoder *this_CurvatureEncoder);

	/**
	 * ‹È—¦”¼Œa‚ðŽæ“¾‚·‚é
	 */
	float CurvatureEncoder_getCurvature(CurvatureEncoder *this_CurvatureEncoder);

};

}  // namespace ‹ì“®•”
#endif
