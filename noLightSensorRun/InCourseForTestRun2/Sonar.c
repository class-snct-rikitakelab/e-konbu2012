#include "Sonar.h"

//�����g�Z���T��Ԍ��o�֐�
int sonarCheck(int target_sonar)
{
	if(sonarvalue <= target_sonar)	//�����g�Z���T�̒l���ڕW�l�ȉ������f���t���O�ύX
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