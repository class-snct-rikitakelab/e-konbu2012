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
				distanceslopestart = getDistance();

			distanceslopeup = getDistance();

			setCmdForward(RA_speed(70));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(distanceslopeup - distanceslopestart > 30 && timecounter > 300)
			{
				ecrobot_sound_tone(880, 512, 50);
				runningslope = SLOPE_TOP;
				timecounter = 0;
			}

			break;

		case (SLOPE_TOP):
			setCmdForward(RA_speed(60));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(getInitGyroOffset() + 30 < (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && timecounter > 300)
			{
				ecrobot_sound_tone(880, 512, 50);
				runningslope = SLOPE_DOWN;
				timecounter = 0;
			}
			break;

		case (SLOPE_DOWN):
			if(timecounter == 0)
				distanceslopetop = getDistance();

			distanceslopedown = getDistance();

			setCmdForward(RA_speed(60));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(distanceslopedown - distanceslopetop > 30 && timecounter > 300)
			{
				runningslope = SLOPE_END;
				timecounter = 0;
				ecrobot_sound_tone(880, 512, 50);
			}
			break;

		case (SLOPE_END):
			setCmdForward(RA_speed(70));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && timecounter > 300)
			{
				ecrobot_sound_tone(940, 512, 30);
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
