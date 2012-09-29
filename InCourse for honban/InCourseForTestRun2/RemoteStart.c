#include "RemoteStart.h"

/*
 *	リモートスタート管理関数
 *	NXTCommunicatorから1を送信でリモートスタート
 *	引数:無し
 *	返り値:
 */
int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//状態フラグ
	char rx_buf[BT_MAX_RX_BUF_SIZE];

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
			start = 1; //走行開始フラグ
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); //受信データをエコーバック
	}

	return start;
}