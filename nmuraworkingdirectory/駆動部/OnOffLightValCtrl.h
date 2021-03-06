#ifndef 駆動部_ON_OFF_LIGHT_VAL_CTRL_H
#define 駆動部_ON_OFF_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "駆動部/LightValCtrlMethod.h"
#include "駆動部/S8.h"
#include "検出部/U16.h"

namespace 駆動部
{
class OnOffLightValCtrl : public LightValCtrlMethod
{
public:
	S8 OnOffLightValCtrl_calcLightValCtrlVal(OnOffLightValCtrl *this_OnOffLightValCtrl, 検出部::U16 targLightVal, 検出部::U16 lightVal);

	void OnOffLightValCtrl_OnOffLightValCtrl_init(OnOffLightValCtrl *this_OnOffLightValCtrl);

};

}  // namespace 駆動部
#endif
