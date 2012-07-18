# Target specific macros
TARGET = Kaidan

NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# nxtway_gs_balancer library desiged for NXTway-GS two wheeled self-balancing robot
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer
USER_LIB = nxtway_gs_balancer

# using NXT standard tires (not Motorcycle tires)
#USER_DEF = NXT_STD_TIRE

# User application source
<<<<<<< HEAD
TARGET_SOURCES = balancer_param.c Kaidan.c logSend.c
=======
TARGET_SOURCES = balancer_param.c Kaidan_story.c logSend.c
>>>>>>> 001f37a76a6e0a1fc19b43244219fd92f6516346
TOPPERS_OSEK_OIL_SOURCE = prog.oil

O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/lejos_osek.tmf
