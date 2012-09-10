#include "Slope.h"

int runningSlope()
{

	static int timecounter = 0;

	static int distanceslopestart = 0;
	static int distanceslopeup = 0;
	static int distanceslopetop = 0;
	static int distanceslopedown = 0;

	int slopeendflag = 0;

	int weight = 1.0;

	switch(runningslope){
		case (SLOPE_START):
			if(timecounter == 0)
				distanceslopestart = getNowDistance();

			distanceslopeup = getNowDistance();

			setCmdForward(RA_speed(80));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));

			if(distanceslopeup - distanceslopestart > 10 && timecounter > 300)
			{
				ecrobot_sound_tone(800, 512, 20);
				runningslope = SLOPE_TOP;
				timecounter = 0;
			}

			break;

		case (SLOPE_TOP):
			setCmdForward(RA_speed(60));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));

			if(getInitGyroOffset() + 30 < (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)/* && timecounter > 300*/)
			{
				ecrobot_sound_tone(820, 512, 20);
				runningslope = SLOPE_DOWN;
				timecounter = 0;
			}
			break;

		case (SLOPE_DOWN):
			if(timecounter == 0)
				distanceslopetop = getNowDistance();

			distanceslopedown = getNowDistance();

			setCmdForward(RA_speed(60));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));

			if(distanceslopedown - distanceslopetop > 30/* && timecounter > 300*/)
			{
				runningslope = SLOPE_END;
				timecounter = 0;
				ecrobot_sound_tone(840, 512, 20);
			}
			break;

		case (SLOPE_END):
			setCmdForward(RA_speed(80));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));
			
			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)/* && timecounter > 300*/)
			{
				ecrobot_sound_tone(900, 512, 20);
				slopeendflag = 1;
				timecounter = 0;
			}

			break;

		default:
			break;
		}
	timecounter++;

	return slopeendflag;
}

int getslope()
{
	return 1;
}
