#ifndef __LightValCtrl
#define __LightValCtrl

#include "ecrobot_interface.h"
#include "../NXTHardWare/LightSensor.h"


/**
 * �P�x�l�����
 */
typedef struct { 

}LightValCtrl;


	void LightValCtrl_init(LightValCtrl *this_LightValCtrl);

	/**
	 * �ڕW�P�x�l��ݒ肷��
	 */
	void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl,U16 targLightVal);

	/**
	 * �P�x�l�ڕW��������s����
	 */
	S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl);


#endif

