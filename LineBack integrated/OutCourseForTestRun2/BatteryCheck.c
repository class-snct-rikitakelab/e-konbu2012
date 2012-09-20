#include "BatteryCheck.h"

//衝撃検知関数
int shock(int target){

	int result=0;

	//電圧降下の最小値を更新
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//ジャイロ及び電圧降下から衝撃検知
	if(target <= battery_value-min_vol)
	{
		result = 1;
	}
	else
		result = 0;

	return result;
}

void setMinVol(int setvalue)
{
	min_vol = setvalue;
}

void setBatteryValue(int setvalue)
{
	battery_value = setvalue;
}

int getbatteryvalue(void)
{
	return battery_value;
}