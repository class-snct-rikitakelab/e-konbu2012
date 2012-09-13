#ifndef �쓮��_P_I_D_CURVATURE_CTRL_H
#define �쓮��_P_I_D_CURVATURE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/PIDCurvatureCtrlParm.h"
#include "�쓮��/PIDCurvatureParm.h"
#include "�쓮��/PIDCtrlParm.h"
#include "DrivenPart/S8.h"

namespace �쓮��
{
/**
 * PID�ȗ����a�������
 */
class PIDCurvatureCtrl
{
private:
	/**
	 * �΍�
	 */
	float deviation;

	float integratedDeviation;

	int lastMeasurementTime;

	PIDCurvatureCtrlParm pIDCurvatureCtrlParm;

public:
	/**
	 * ����ʂ��Z�o����
	 */
	DrivenPart::S8 PIDCurvatureCtrl_calcCurvatureCtrlVal(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

	/**
	 * �ȗ����a����p�����[�^���擾����
	 */
	PIDCurvatureParm PIDCurvatureCtrl_getPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

	/**
	 * �ȗ����a����p�����[�^��ݒ肷��
	 */
	void PIDCurvatureCtrl_setPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl, PIDCtrlParm parm);

	void PIDCurvatureCtrl_init(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

};

}  // namespace �쓮��
#endif
