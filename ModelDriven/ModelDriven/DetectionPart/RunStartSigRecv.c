#include "RunStartSigRecv.h"

void RunStartSigRecv_init(RunStartSigRecv *this_RunStartSigRecv){

}

	/**
	 * ���i�M�������m����
	 */
BOOL RunStartSigRecv_detcRunStartSig(RunStartSigRecv *this_RunStartSigRecv){
	char rx_buf[BT_MAX_RX_BUF_SIZE];

	int i;
	unsigned int rx_len;
	 BOOL start = false; //�J�n�M����M�̎�肱�ڂ��̉\��������ꍇstatic�ϐ��ɂ���Ƃ悢����

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
			
			start = true; //���s�J�n�t���O
		}
	}
	
	systick_wait_ms(20);	//�f�o�b�O�p�i�^�b�`�Z���T�҂��j

	//�^�b�`�Z���T�X�^�[�g
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

