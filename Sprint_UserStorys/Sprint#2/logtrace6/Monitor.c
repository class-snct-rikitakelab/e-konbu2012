

TASK(DisplayTask)
{
	/* ���j�^�ɃV�X�e������\������ */
	ecrobot_status_monitor(target_subsystem_name);

	/* ���^�X�N�̏I�� */
	/* ��̓I�ɂ́C���^�X�N�����s��Ԃ���x�~��ԂɈڍs�����C*/
	/* �^�X�N�̏I�����ɍs���ׂ��������s�� */
	TerminateTask();
}