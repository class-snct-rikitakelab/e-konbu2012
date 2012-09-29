#include "Sonar.h"

//超音波センサ状態検出関数
int sonarCheck(int target_sonar)
{
	if(sonarvalue <= target_sonar)	//超音波センサの値が目標値以下か判断しフラグ変更
	{
		return 1;
	}
	else
		return 0;
}

void getSonarValue(void)
{
	sonarvalue = ecrobot_get_sonar_sensor(NXT_PORT_S2);
}