#
# Makefile
#
# Target: GenLib
#
# Environment
#
WORK_INC = $(WORK_HOME)/inc/
WORK_LIB = $(WORK_HOME)/lib/
WORK_BIN = $(WORK_HOME)/bin/
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
OBJS = CLSprcthrc.o CLSprcmthr.o
#
# Target Process
#
TARGET = libYgd.a
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(AR) $@ $(OBJS) $(LIBS)
	@mv $@ $(WORK_LIB)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_LIB)$(TARGET) $(TARGET) $(OBJS)
	