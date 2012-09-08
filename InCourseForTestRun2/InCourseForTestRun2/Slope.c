#include "Slope.h"

int runningSlope()
{

	static int timecounter = 0;

	int distanceslopestart;
	int distanceslopeup;
	int distanceslopetop;
	int distanceslopedown;
	int distanceslopeend;

	int slopeendflag = 0;

	switch(runningslope)
	{
		case (SLOPE_START):
			if(timecounter == 0)
				distanceslopestart = getDistance();

			distanceslopeup = getDistance();

			setCmdForward(RA_speed(75));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(distanceslopeup - distanceslopestart > 30)
			{
				runningslope = SLOPE_DOWN;
				timecounter = 0;
			}

			break;

		case (SLOPE_TOP):
			setCmdForward(RA_speed(55));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				ecrobot_sound_tone(880, 512, 30);
				runningslope = SLOPE_DOWN;
				timecounter = 0;
			}
			break;

		case (SLOPE_DOWN):
			if(timecounter == 0)
				distanceslopetop = getDistance();

			distanceslopedown = getDistance();

			setCmdForward(RA_speed(55));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(distanceslopedown - distanceslopetop > 30)
			{
				runningslope = SLOPE_END;
				timecounter = 0;
			}
			break;

		case (SLOPE_END):
			setCmdForward(RA_speed(75));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				ecrobot_sound_tone(880, 512, 30);
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