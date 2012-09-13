#ifndef __LightValCtrl
#define __LightValCtrl

#include "ecrobot_interface.h"
#include "../NXTHardWare/LightSensor.h"


/**
 * 輝度値制御器
 */
typedef struct { 

}LightValCtrl;


	void LightValCtrl_init(LightValCtrl *this_LightValCtrl);

	/**
	 * 目標輝度値を設定する
	 */
	void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl,U16 targLightVal);

	/**
	 * 輝度値目標制御を実行する
	 */
	S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl);


#endif

