#include "RunStartSigRecv.h"

void RunStartSigRecv_init(RunStartSigRecv *this_RunStartSigRecv){

}

	/**
	 * 発進信号を検知する
	 */
BOOL RunStartSigRecv_detcRunStartSig(RunStartSigRecv *this_RunStartSigRecv){
	char rx_buf[BT_MAX_RX_BUF_SIZE];

	int i;
	unsigned int rx_len;
	 BOOL start = false; //開始信号受信の取りこぼしの可能性がある場合static変数にするとよいかも

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; //受信バッファをクリア
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		//受信データあり
		if (rx_buf[0] == CMD_START)
		{
			
			start = true; //走行開始フラグ
		}
	}
	
	systick_wait_ms(20);	//デバッグ用（タッチセンサ待ち）

	//タッチセンサスタート
	if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
	{
		ecrobot_sound_tone(200,512,10);
		start = true;
	}
	//HOME BUTTON START FOR DEBG at HOME
	if (ecrobot_is_ENTER_button_pressed() == TRUE)
	{
		systick_wait_ms(20);
		start = true;
	}
	return start;
	}

