#ifndef __TailAngleCtrl
#define __TailAngleCtrl

/**
 * �K���p�x�����
 */
typedef struct  
{

}TailAngleCtrl;

	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	/**
	 * �ڕW�p�x��ݒ肷��
	 */
	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int agnle);

	/**
	 * �p�x�ڕW��������s����
	 */
	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

#endif

