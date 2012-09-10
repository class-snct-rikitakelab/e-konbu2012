#include "RemoteStart.h"

/*
 *	�����[�g�X�^�[�g�Ǘ��֐�
 *	NXTCommunicator����1�𑗐M�Ń����[�g�X�^�[�g
 *	����:����
 *	�Ԃ�l:
 */
int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//��ԃt���O
	char rx_buf[BT_MAX_RX_BUF_SIZE];

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
		ecrobot_send_bt(rx_buf, 0, rx_len); //��M�f�[�^���G�R�[�o�b�N
	}

	return start;
}


int RN_rapid_speed_up_signal_recevie(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;

	char rx_buf[BT_MAX_RX_BUF_SIZE];

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* ��M�o�b�t�@���N���A */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	
	if (rx_len > 0)
	{
		/* ��M�f�[�^���� */
		if (rx_buf[0] == rapid_SPEED_UP_SIGNAL)
		{
			start = 1;
		}
		
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		start = 1; 

	return start;
}
