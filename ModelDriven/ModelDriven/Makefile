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
	StrategyPart/CngSectTerm.c \
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
	DirectorPart/CngSectLisnner.c \
	Calibration.c \
	Factory.c \
	PIDCalcDebug.c \
	PWMValGenerator.c \
	WLReceiver.c \
	TailControl.c \
	SelfLocation.c \

TOPPERS_OSEK_OIL_SOURCE := prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf

