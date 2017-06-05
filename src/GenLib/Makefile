#
# Makefile
#
# Target: CLSFILETEST
#
# Environment
#
WORK_INC = /home/ygd/test/work/GD/inc/
WORK_LIB = /home/ygd/test/work/GD/lib/
WORK_BIN = /home/ygd/test/work/GD/bin/
#

#
#----------------------------------------
# $@ : TARGET NAME
# $* : TARGET name except .*
# $< : first item in dependency list
# $? : last renewar file than current target file
#----------------------------------------

#
# Rules
#
DEFINES =
CCOPTS = -c -Wno-write-strings -I$(WORK_INC)
LDOPTS = $(DEFINES) -L$(WORK_LIB)
AROPTS = rcv

AR = ar $(AROPTS)
CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o : ; $(CC) $<
#
# Libraries used for linking
#
LIBS =
#
# Library files used for dependency tests
#
LIB_DEPS =
#
# Object files
#
# Object files
#
OBJS = CLSobject.o CLStbObject.o CLSrunObject.o CLSmemory.o CLSprocess.o CLSthread.o CLScomm.o CLStcp.o CLSlog.o CLSsystem.o
#OBJS = CLStbObject.o CLSrunObject.o test.o
#
# Target Process
#
TARGET = libSane.a
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(AR) $@ $(OBJS) $(LIBS)
	#@mv CLSFILETEST $(WORK_BIN)
	@mv $@ $(WORK_LIB)
	#@rm -rf  $(OBJS)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_LIB)$(TARGET) $(TARGET) $(OBJS)
	