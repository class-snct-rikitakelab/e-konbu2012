#ifndef �쓮��_ON_OFF_LIGHT_VAL_CTRL_H
#define �쓮��_ON_OFF_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/LightValCtrlMethod.h"
#include "�쓮��/S8.h"
#include "���o��/U16.h"

namespace �쓮��
{
class OnOffLightValCtrl : public LightValCtrlMethod
{
public:
	S8 OnOffLightValCtrl_calcLightValCtrlVal(OnOffLightValCtrl *this_OnOffLightValCtrl, ���o��::U16 targLightVal, ���o��::U16 lightVal);

	void OnOffLightValCtrl_OnOffLightValCtrl_init(OnOffLightValCtrl *this_OnOffLightValCtrl);

};

}  // namespace �쓮��
#endif
