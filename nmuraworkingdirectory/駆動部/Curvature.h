#ifndef ‹ì“®•”_CURVATURE_H
#define ‹ì“®•”_CURVATURE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/CurvatureEncoder.h"

namespace ‹ì“®•”
{
/**
 * ‹È—¦”¼Œa
 */
class Curvature
{
private:
	/**
	 * ‹È—¦”¼Œa–Ú•W’l
	 */
	float targCurvature;

	CurvatureEncoder curvatureEncoder;

public:
	/**
	 * ‹È—¦”¼Œa‚ğæ“¾‚·‚é
	 */
	float Curvature_getCurvature();

	/**
	 * ‹È—¦”¼Œa–Ú•W’l‚ğİ’è‚·‚é
	 */
	float Curvature_getTargCurvarture();

	/**
	 * ‹È—¦”¼Œa–Ú•W’l‚ğİ’è‚·‚é
	 */
	void Curvature_setTargCurvature(float targCurvature);

};

}  // namespace ‹ì“®•”
#endif
