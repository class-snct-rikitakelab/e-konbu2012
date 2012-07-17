#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */
#include "logSend.h"
#include "Maimai.h"
#include <stdlib.h>



float Maimai(){
static unsigned int counter_maimai=0;
U16 lightDowned;
U16 lightUpped;
U16 lightDiff;
float k;
static float brightness;
//static float brighteness,buf_brighness;

++counter_maimai;
if(counter_maimai==20/4){//約20ms
	//赤色LED消灯時のセンサ値を取得
	lightDowned=ecrobot_get_light_sensor(NXT_PORT_S3);
	//赤色LEDを点灯する
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	}

if(counter_maimai==40/4){//約40ms
	//赤色LED点灯時のセンサ値を取得
	lightUpped=ecrobot_get_light_sensor(NXT_PORT_S3);
	//赤色LEDを消灯する
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	//カウンタをクリア
	counter_maimai=0;
}

//光センサの変化量を計算

if(lightDowned - lightUpped>0){
	lightDiff=lightDowned - lightUpped;
}
else{
	lightDiff=0U;
}

k=(1.0382E-3 * lightDowned -6.3295E-1)*lightDowned + 1.1024E+2;
brightness = (float)lightDiff/k;

return brightness;


}