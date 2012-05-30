typedef enum{
	RN_SETTINGMODE_GYRO_START,
	RN_SETTINGMODE_END
} RN_SETTINGMODE;

typedef enum{
	RN_MODE_INIT, 		/* system initialize mode */
	RN_MODE_CONTROL,		/* control mode */
	RN_RUN
} RN_MODE;

void RN_setting();

TASK(ActionTask2)
{
	RN_setting();

	/* ���^�X�N�̏I�� */
	/* ��̓I�ɂ́C���^�X�N�����s��Ԃ���x�~��ԂɈڍs�����C*/
	/* �^�X�N�̏I�����ɍs���ׂ��������s�� */
	TerminateTask();
}

void RN_setting()
{
	switch (setting_mode){
		case (RN_SETTINGMODE_GYRO_START):
			RN_calibrate();
			break;

		case (RN_SETTINGMODE_END):
			runner_mode = RN_MODE_CONTROL;
			setting_mode=RN_RUN;

		case (RN_RUN):
			RA_linetrace_P(30);
			break;
		default:
			break;
	}
}