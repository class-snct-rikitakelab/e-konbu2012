# Target specific macros
TARGET = InCourse

NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# nxtway_gs_balancer library desiged for NXTway-GS two wheeled self-balancing robot
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer
USER_LIB = nxtway_gs_balancer

# using NXT standard tires (not Motorcycle tires)
#USER_DEF = NXT_STD_TIRE

# User application source
TARGET_SOURCES := \
	balancer_param.c \
	MainRunningInCourse.c \
	Calibration.c \
	LightPIDControl.c \
	SelfLocation.c \
	SpeedControl.c \
	TailControl.c \
	RemoteStart.c \
	LogSend.c \
	PWMGenerator.c \
	Slope.c \
	CalcDistance.c \
	LookUpGate.c \
	Sonar.c \
	MarkerDetector.c \
	mymath.c \
	runnerForTestRun.c \
	NXTButton.c \
	tyreal_light_ver.c \
	LightSensor.c \


TOPPERS_OSEK_OIL_SOURCE := ./prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf

