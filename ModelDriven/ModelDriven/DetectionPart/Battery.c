
#include "Battery.h"

void Battery_init(Battery *this_Battery){

}
BOOL Battery_dectRedct(Battery *this_Battery){
	BOOL res=false;

	if(this_Battery->initVol - ecrobot_get_battery_voltage() > this_Battery->targReductRang)
		res = true;
}

