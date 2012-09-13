#ifndef __TailAngleCtrl
#define __TailAngleCtrl

/**
 * 尻尾角度制御器
 */
typedef struct  
{

}TailAngleCtrl;

	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	/**
	 * 目標角度を設定する
	 */
	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int agnle);

	/**
	 * 角度目標制御を実行する
	 */
	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

#endif