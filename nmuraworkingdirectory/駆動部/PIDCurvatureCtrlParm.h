#ifndef �쓮��_P_I_D_CURVATURE_CTRL_PARM_H
#define �쓮��_P_I_D_CURVATURE_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace �쓮��
{
/**
 * PID�ȗ����a����p�����[�^
 */
class PIDCurvatureCtrlParm
{
private:
	/**
	 * ���W��
	 */
	float cKp;

	/**
	 * �ϕ��W��
	 */
	float cKi;

	/**
	 * �����W��
	 */
	float cKd;


public:
	/**
	 * ���W�����擾����
	 */
	float PIDCurvatureCtrlParm_getCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * ���W����ݒ肷��
	 */
	float PIDCurvatureCtrlParm_setCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

	/**
	 * �ϕ��W����ݒ肷��
	 */
	float PIDCurvatureCtrlParm_getCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * �ϕ��W�����擾����
	 */
	float PIDCurvatureCtrlParm_setCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

	/**
	 * �����W�����擾����
	 */
	float PIDCurvatureCtrlParm_getCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * �ϕ��W����ݒ肷��
	 */
	float PIDCurvatureCtrlParm_setCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

};

}  // namespace �쓮��
#endif
