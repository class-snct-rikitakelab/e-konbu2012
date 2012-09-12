
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/路面輝度値.h"
#include "DrivenPart/輝度値制御方式.h"
#include "DrivenPart/S8.h"
#include "DetectionPart/U16.h"
#include "NXTHardWare/LightSensor.h"

namespace DrivenPart
{
/**
 * 輝度値制御器
 */
class LightValCtrl
{
private:
	路面輝度値 路面輝度値;
	輝度値制御方式 輝度値制御方式;
	NXTHardWare::LightSensor lightSensor;

public:
	void LightValCtrl_init(LightValCtrl *this_LightValCtrl);

	/**
	 * 目標輝度値を設定する
	 */
	void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl, DetectionPart::U16 targLightVal);

	/**
	 * 輝度値目標制御を実行する
	 */
	S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl);

};

}  // namespace DrivenPart
