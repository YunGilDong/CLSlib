#
# Makefile
#
# Target: PRC_THR
#
# Environment
#
WORK_INC =  $(WORK_HOME)/inc/
WORK_INC2 = $(WORK_HOME)/src/PRC_THR/
WORK_LIB =  $(WORK_HOME)/lib/
WORK_BIN =  $(WORK_HOME)/bin/
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
CCOPTS = -c -I$(WORK_INC) -I$(WORK_INC2)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
#LIBS = -lpthread -lSane
LIBS = -pthread -lSane -lYgd
#
#
# Object files
#
OBJS = THRtest.o PRC_THR.o
#
# Target Process
#
TARGET = PRC_THR
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	@mv $@ $(WORK_BIN)
	#@rm -rf  $(OBJS)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_BIN)$(TARGET) $(TARGET) $(OBJS)
