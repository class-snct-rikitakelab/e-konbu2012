/*
	ライントレース関係
*/

void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_p(int forward_speed);



void RA_run(int flag){
	/*走行状態*/
	if(flag == 0)
		RA_linetrace(30,100);
	else if(flag == 1)
		RA_linetrace_P(30);
}

void RA_stop(void){
	/*停止状態*/
	cmd_forward = 0;
	cmd_turn = 0;
}


void RA_linetrace(int forward_speed, int turn_speed) {
	/*PID制御を用いらないライントレース*/
	if(ecrobot_get_battery_voltage() <= battery_value-400)
		{cmd_forward = forward_speed*3;
		ecrobot_sound_tone(880, 512, 30);}
	else if(ecrobot_get_battery_voltage() >= battery_value+400)
		{cmd_forward = forward_speed/3;
		ecrobot_sound_tone(900,512,30);}
//	else if(ecrobot_get_battery_voltage() <= battery_value-400)
//		ecrobot_sound_tone(890,512,30);
//	else if(ecrobot_get_battery_voltage() <= battery_value-300)
//		ecrobot_sound_tone(900,512,30);
//	else if(ecrobot_get_battery_voltage() <= battery_value-200)
//		ecrobot_sound_tone(910,512,30);
//	else
	else	cmd_forward = forward_speed;

	/* 白黒の判断をして回転方向を決める */
	int light_value = 0;
	light_value = online();
	if (TRUE != light_value) {
		cmd_turn = (-1)*turn_speed;
	} else {
		cmd_turn = turn_speed;
	}

//	battery_value = ecrobot_get_battery_voltage();

}

void RA_linetrace_P(int forward_speed) {
	/*PID制御を用いたライントレース*/

	static float hensa;
	static float i_hensa = 0;
	static float d_hensa = 0;
	static float bf_hensa = 0;
	static float Kp = 1.75;
	static float Ki = 2.6;
	static float Kd = 0.01;

	if(ecrobot_get_battery_voltage() <= battery_value-550)
		{//cmd_forward = 0;
		ecrobot_sound_tone(880, 512, 30);
	}}

//	else if(ecrobot_get_battery_voltage() >= battery_value+400)
//		{//cmd_forward = forward_speed/3;
//		ecrobot_sound_tone(900,512,30);}

	cmd_forward = forward_speed;

	if(forward_speed<0)
		forward_speed*=(-1);

	hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	i_hensa = i_hensa+(hensa*0.0004);
	d_hensa = (hensa - bf_hensa)/0.0004;
	bf_hensa = hensa;
	/* 白いと＋値 */
	/* 黒いと－値 */
	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}
	battery_value = ecrobot_get_battery_voltage();

}

int online(void) {
	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);
	if (GRAY_VALUE > light_value) {
		if ((GRAY_VALUE - 20) > light_value) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
	return TRUE;
}
