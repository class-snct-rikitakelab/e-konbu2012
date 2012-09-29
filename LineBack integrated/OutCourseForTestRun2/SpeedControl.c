#include "SpeedControl.h"

//段階的加速用関数（指定量だけ速度を徐々に上昇）
int RA_speed(int forward_speed){

	static int result_speed = 0;
	static int counter = 0;
	
	counter++;

	if(counter >= SPEED_COUNT)
	{
		if(forward_speed - result_speed >= 0){
			result_speed++;

			if(result_speed > forward_speed)
				result_speed = forward_speed;
		}
		else{
			result_speed--;

			if(result_speed < forward_speed)
				result_speed = forward_speed;
		}
		counter = 0;
	}

	return result_speed;
}
