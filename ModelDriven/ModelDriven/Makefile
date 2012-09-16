# Target specific macros
TARGET = ObjectOriented

NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# nxtway_gs_balancer library desiged for NXTway-GS two wheeled self-balancing robot
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer
USER_LIB = nxtway_gs_balancer

# using NXT standard tires (not Motorcycle tires)
#USER_DEF = NXT_STD_TIRE

# User application source
TARGET_SOURCES := \
	balancer_param.c \
	Slope.c \
	Common/Factory.c \
	Common/mymath.c \
	StrategyPart/CngSectTerm.c \
	StrategyPart/Sector.c \
	StrategyPart/TargetDrivenParm.c \
	DetectionPart/RunDist.c \
	DetectionPart/TurnAngle.c \
	DetectionPart/Coordinates.c \
	DetectionPart/DetcObst.c \
	DetectionPart/RunStartSigRecv.c \
	DetectionPart/LineEdge.c \
	DetectionPart/DetcMarker.c \
	DetectionPart/Gradient.c \
	DetectionPart/Timer.c \
	DetectionPart/Battery.c \
	DetectionPart/Notice.c \
	DetectionPart/LocationInfo.c \
	DirectorPart/Running.c \
	DirectorPart/RunnerRobot.c \
	DirectorPart/CngSectLisnner.c \
	DrivenPart/CurvatureCtrl.c \
	DrivenPart/ForwardValRevise.c \
	DrivenPart/LightValCtrl.c \
	DrivenPart/RobotDrivenDirect.c \
	DrivenPart/TailAngleCtrl.c \
	DrivenPart/WheelMotor.c \
	DrivenPart/WheelMotorCtrl.c \
	DrivenPart/LightVal.c \
	DrivenPart/LightValCtrl.c \
	DrivenPart/LightValCtrlMethod.c \
	DrivenPart/LightValCtrlParm.c \
	DrivenPart/PIDLightValCtrl.c \
	DrivenPart/PIDLightValCtrlParm.c \
	DrivenPart/PIDTailAngleCtrl.c \
	DrivenPart/PIDTailAngleCtrlParm.c \
	DrivenPart/TailAngle.c \
	DrivenPart/TailAngleCtrl.c \
	DrivenPart/TailMotor.c \
	DrivenPart/TailMotorEncoder.c \
	NXTHardWare/GyroSensor.c \
	NXTHardWare/LightSensor.c \
	NXTHardWare/Sound.c \
	NXTHardWare/TouchSensor.c \
	NXTHardWare/MotorEncoder.c \
	NXTHardWare/SonorSensor.c \
	TestDrivenPart.c 
	

TOPPERS_OSEK_OIL_SOURCE := prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf

