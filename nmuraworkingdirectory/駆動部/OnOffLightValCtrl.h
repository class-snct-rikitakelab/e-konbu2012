#ifndef ‹ì“®•”_ON_OFF_LIGHT_VAL_CTRL_H
#define ‹ì“®•”_ON_OFF_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/LightValCtrlMethod.h"
#include "‹ì“®•”/S8.h"
#include "ŒŸo•”/U16.h"

namespace ‹ì“®•”
{
class OnOffLightValCtrl : public LightValCtrlMethod
{
public:
	S8 OnOffLightValCtrl_calcLightValCtrlVal(OnOffLightValCtrl *this_OnOffLightValCtrl, ŒŸo•”::U16 targLightVal, ŒŸo•”::U16 lightVal);

	void OnOffLightValCtrl_OnOffLightValCtrl_init(OnOffLightValCtrl *this_OnOffLightValCtrl);

};

}  // namespace ‹ì“®•”
#endif
