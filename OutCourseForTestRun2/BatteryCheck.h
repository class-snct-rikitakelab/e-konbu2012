#ifndef _BATTERYCHECK_H_
#define _BATTERYCHECK_H_

//バッテリ降下値
#define STEP_BATTERY 300
#define STOP_BATTERY 400

#include "ecrobot_interface.h"

static int min_vol;

static int battery_value;

extern int shock(int target);

extern void setMinVol();
extern void setMinVol(int setvalue);
extern int getbatteryvalue(void);
extern void setBatteryValue(int setvalue);


#endif