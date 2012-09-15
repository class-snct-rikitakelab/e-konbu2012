# Target specific macros
TARGET = e-konbu

NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# nxtway_gs_balancer library desiged for NXTway-GS two wheeled self-balancing robot
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer
USER_LIB = nxtway_gs_balancer

# using NXT standard tires (not Motorcycle tires)
#USER_DEF = NXT_STD_TIRE

# User application source
TARGET_SOURCES := \
	../DrivenPart/TailMotor.c \
	../DrivenPart/TailAngle.c \
	../DrivenPart/TailAngleCtrl.c \
	../DrivenPart/TailMotorEncoder.c \
	../DrivenPart/PIDTailAngleCtrl.c \
	../DrivenPart/PIDTailAngleCtrlParm.c \
	../Common/Factory.c \
	../LogSend.c \
	../DrivenPart/LightValCtrl.c \
	../DrivenPart/LightVal.c \
	../DrivenPart/PIDLightValCtrl.c \
	../DrivenPart/PIDLightValCtrlParm.c \
	../NXTHardware/LightSensor.c \
	../DrivenPart/CurvatureCtrl.c \
	../DrivenPart/Curvature.c \
	../DrivenPart/PIDCurvatureCtrl.c \
	../DrivenPart/PIDCurvatureCtrlParm.c \
	../DrivenPart/CurvatureEncoder.c \
	../DrivenPart/ForwardValRevise.c \
	../TestCode.c

TOPPERS_OSEK_OIL_SOURCE := prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf

