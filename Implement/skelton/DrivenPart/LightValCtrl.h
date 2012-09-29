
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/�H�ʋP�x�l.h"
#include "DrivenPart/�P�x�l�������.h"
#include "DrivenPart/S8.h"
#include "DetectionPart/U16.h"
#include "NXTHardWare/LightSensor.h"

namespace DrivenPart
{
/**
 * �P�x�l�����
 */
class LightValCtrl
{
private:
	�H�ʋP�x�l �H�ʋP�x�l;
	�P�x�l������� �P�x�l�������;
	NXTHardWare::LightSensor lightSensor;

public:
	void LightValCtrl_init(LightValCtrl *this_LightValCtrl);

	/**
	 * �ڕW�P�x�l��ݒ肷��
	 */
	void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl, DetectionPart::U16 targLightVal);

	/**
	 * �P�x�l�ڕW��������s����
	 */
	S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl);

};

}  // namespace DrivenPart
