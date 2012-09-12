
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
	/**
	 * �ڕW�ȗ����a��ݒ肷��
	 */
	void CurvatureCtrl_setTargCurature(int *this_CurvatureCtrl, float targCurvature);

	/**
	 * �ȗ����a�ڕW��������s����
	 */
	S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

};

}  // namespace DrivenPart
