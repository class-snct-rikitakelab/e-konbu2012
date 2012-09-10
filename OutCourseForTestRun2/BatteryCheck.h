#ifndef _BATTERYCHECK_H_
#define _BATTERYCHECK_H_

//バッテリ降下値
#define STEP_BATTERY 300
#define STOP_BATTERY 400

#include "ecrobot_interface.h"

static int min_vol = ecrobot_get_battery_voltage();

extern int shock(int target);

extern void setMinVol();
void setMinVol(int setvalue)

#endif