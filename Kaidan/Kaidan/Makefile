# Target specific macros
TARGET = Kaidan

NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# nxtway_gs_balancer library desiged for NXTway-GS two wheeled self-balancing robot
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer
USER_LIB = nxtway_gs_balancer

# using NXT standard tires (not Motorcycle tires)
#USER_DEF = NXT_STD_TIRE

# User application source
TARGET_SOURCES := \
		balancer_param.c \
		Kaidan.c \
<<<<<<< HEAD
		logSend.c \
		tyreal_light_ver.c
=======
		logSend.c
>>>>>>> 62b9c9b5a83d0501627b072f2661f9e32ddc334b

TOPPERS_OSEK_OIL_SOURCE = prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf
