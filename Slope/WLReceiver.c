/*
 *	������M��N���X
 */

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "WLReceiver.h"

void WLReceiver_init(WLReceiver * this_WLReceiver)
{
}

//�����[�g�X�^�[�g�Ǘ��֐�
int WLReceiver_RunningOperationReceive(WLReceiver * this_WLReceiver)
{
	char rx_buf[BT_MAX_RX_BUF_SIZE];

	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//��ԃt���O

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; //��M�o�b�t�@���N���A
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		//��M�f�[�^����
		if (rx_buf[0] == CMD_START)
		{
			start = 1; //���s�J�n�t���O
		}
	}
	
	systick_wait_ms(500);	//�f�o�b�O�p�i�^�b�`�Z���T�҂��j

	//�^�b�`�Z���T�X�^�[�g
	if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
	{
		ecrobot_sound_tone(982,512,10);
		start = 1;
	}
	return start;
}
/*
void WLReceiver_setWLReceiverState(WLReceiver * this_WLReceiver,WLState state)
{
	this_WLReceiver -> state = state;
}

WLState WLReceiver_getWLReceiverState(WLReceiver * this_WLReceiver)
{
	return this_WLReceiver->state;
}
*/
