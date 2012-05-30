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

	/* 自タスクの終了 */
	/* 具体的には，自タスクを実行状態から休止状態に移行させ，*/
	/* タスクの終了時に行うべき処理を行う */
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