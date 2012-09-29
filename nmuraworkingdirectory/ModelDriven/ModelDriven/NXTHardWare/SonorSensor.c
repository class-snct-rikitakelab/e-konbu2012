

#include "SonorSensor.h"

void SonorSensor_init(SonorSensor *this_SonorSensor,S32 port)
{
	this_SonorSensor->port=port;
}


S32 SonorSensor_getSonor(SonorSensor *this_SonorSensor)
{	
	static S32 res=255;
	static int counter=0;
	++counter;
	if(counter==50/10){ //execute 50ms cycle in 10ms cycle task
	res=ecrobot_get_sonar_sensor(this_SonorSensor->port);
	}
	return res;
	
}



