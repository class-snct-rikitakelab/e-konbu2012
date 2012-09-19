#include "Slope.h"

int runningSlope()
{

	static int timecounter = 0;

	static int distanceslopestart = 0;
	static int distanceslopeup = 0;
	static int distanceslopetop = 0;
	static int distanceslopedown = 0;

	int slopeendflag = 0;

	switch(runningslope){
		case (SLOPE_START):
			if(timecounter == 0)
				distanceslopestart = getNowDistance();

			//TailAngleChange(ANGLEOFSLOPEDOWN);

			distanceslopeup = getNowDistance();

			setCmdForward(RA_speed(60));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(distanceslopeup - distanceslopestart > 20 && timecounter > 300)
			{
				ecrobot_sound_tone(800, 512, 50);
				runningslope = SLOPE_TOP;
				timecounter = 0;
				TailAngleChange(ANGLEOFDOWN);
			}

			break;

		case (SLOPE_TOP):
			timecounter++;
			setCmdForward(RA_speed(50));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(getInitGyroOffset() + 50 < (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				TailAngleChange(ANGLEOFSLOPEUP);
				ecrobot_sound_tone(820, 512, 50);
				runningslope = SLOPE_DOWN;
				timecounter = 0;
			}
			break;

		case (SLOPE_DOWN):
			if(timecounter == 0)
				distanceslopetop = getNowDistance();

			distanceslopedown = getNowDistance();

			setCmdForward(RA_speed(50));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(distanceslopedown - distanceslopetop > 50 && timecounter > 800)
			{
				runningslope = SLOPE_END;
				timecounter = 0;
				ecrobot_sound_tone(840, 512, 50);
			}
			break;

		case (SLOPE_END):
			setCmdForward(RA_speed(50));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)/* && timecounter > 300*/)
			{
				TailAngleChange(ANGLEOFDOWN);
				ecrobot_sound_tone(900, 512, 30);
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
