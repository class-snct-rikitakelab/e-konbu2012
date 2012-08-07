static unsigned int LIGHT_THRESHOLD=580;
static unsigned int BLACK_VALUE=600;
static unsigned int WHITE_VALUE=500;
static unsigned int GRAY_VALUE=550;

void CA_calibrate();

void CA_calibrate()
{
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;
	while(1){
	if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
		gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S4);
		break;
		}
	}
	while(1){
	/* バンパを離すと次の状態に遷移する(設定モードを終了する）*/
	if (ecrobot_get_touch_sensor(NXT_PORT_S1) != TRUE) {
		ecrobot_sound_tone(900, 512, 30);
		setting_mode = RN_SETTINGMODE_END;
		break;
		}
	}
}