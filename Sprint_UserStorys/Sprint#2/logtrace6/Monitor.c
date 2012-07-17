

TASK(DisplayTask)
{
	/* モニタにシステム名を表示する */
	ecrobot_status_monitor(target_subsystem_name);

	/* 自タスクの終了 */
	/* 具体的には，自タスクを実行状態から休止状態に移行させ，*/
	/* タスクの終了時に行うべき処理を行う */
	TerminateTask();
}