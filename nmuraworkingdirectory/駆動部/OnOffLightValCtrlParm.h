#ifndef ‹ì“®•”_ON_OFF_LIGHT_VAL_CTRL_PARM_H
#define ‹ì“®•”_ON_OFF_LIGHT_VAL_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/LightValCtrlParm.h"

namespace ‹ì“®•”
{
class OnOffLightValCtrlParm : public LightValCtrlParm
{
private:
	int onThreshold;

	int offThreshold;


public:
	float OnOffLightValCtrlParm_getOnThreshold(OnOffLightValCtrlParm *this_OnOffLightValCtrlParm);

	void OnOffLightValCtrlParm_setOnThreshold(OnOffLightValCtrlParm *this_OnOffLightValCtrlParm, int parm);

	float OnOffLightValCtrlParm_getOffThreshold(OnOffLightValCtrlParm *this_OnOffLightValCtrlParm);

	void OnOffLightValCtrlParm_setOffThreshold(OnOffLightValCtrlParm *this_OnOffLightValCtrlParm, int parm);

};

}  // namespace ‹ì“®•”
#endif
