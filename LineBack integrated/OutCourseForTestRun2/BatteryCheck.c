#include "BatteryCheck.h"

//�Ռ����m�֐�
int shock(int target){

	int result=0;

	//�d���~���̍ŏ��l���X�V
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//�W���C���y�ѓd���~������Ռ����m
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