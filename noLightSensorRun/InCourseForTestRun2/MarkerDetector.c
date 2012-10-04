#include "MarkerDetector.h"

//マーカー検知 v2
S8 markerDetector(){
	//輝度値の変化の二回微分
	static S16 LV_buf = 0;
	static S8 dev_buf[5]  = { 0, 0, 0, 0, 0};
	static S8 dev2_buf[2] = { 0, 0};
	
	S16 LV = ecrobot_get_light_sensor(NXT_PORT_S3);
	S8 dev = LV - LV_buf;
	S8 dev2 = dev - dev_buf[0];

	//マーカー検知アルゴリズム
	S8 mkflg = 0;
	if(dev2_buf[0] <= -15 && (dev2_buf[1] >= 5 || dev2 >= 5)){
		mkflg = 1;
	}
	else {
		mkflg = 0;
	}

	LV_buf		= LV;
	dev_buf[4]	= dev_buf[3];
	dev_buf[3]	= dev_buf[2];
	dev_buf[2]	= dev_buf[1];
	dev_buf[1]	= dev_buf[0];
	dev_buf[0]	= dev;
	dev2_buf[1] = dev2_buf[0];
	dev2_buf[0] = dev2;
	
	return mkflg;
}