#
# Makefile
#
# Target: CPVIMS
#
# Environment
#
WORK_INC =  $(WORK_HOME)/inc/
WORK_LIB =  $(WORK_HOME)/lib/
WORK_BIN =  $(WORK_HOME)/bin/
#abc


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
CCOPTS = -c -I$(WORK_INC)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
#LIBS = -pthread -lSane
LIBS = -lYgd -lSane -pthread
#
#
# Object files
#
#OBJS = CPVIMS.o THRserver.o CLSmap.o
OBJS = CLSmap.o
#
# Target Process
#
TARGET = CPVIMS
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	#@mv $@ $(WORK_BIN)
	#@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_BIN)$(TARGET) $(OBJS) $(TARGET)
