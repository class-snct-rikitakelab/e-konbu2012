# Target specific macros
TARGET = PID

NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# nxtway_gs_balancer library desiged for NXTway-GS two wheeled self-balancing robot
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer
USER_LIB = nxtway_gs_balancer

# using NXT standard tires (not Motorcycle tires)
#USER_DEF = NXT_STD_TIRE

# User application source
TARGET_SOURCES := \
	balancer_param.c \
	PID_parameter.c \
	LogSend.c \
	tyreal_light_ver.c \
	NXTButton.c \
	TailControl.c

TOPPERS_OSEK_OIL_SOURCE := ./prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf

