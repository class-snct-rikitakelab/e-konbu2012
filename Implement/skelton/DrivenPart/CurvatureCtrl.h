
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace DrivenPart
{
/**
 * �ȗ����a�����
 * 
 */
class CurvatureCtrl
{
public:
	void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl);

	/**
	 * �ڕW�ȗ����a��ݒ肷��
	 */
	void CurvatureCtrl_setTargCurature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature);

	/**
	 * �ȗ����a�ڕW��������s����
	 */
	S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

};

}  // namespace DrivenPart
