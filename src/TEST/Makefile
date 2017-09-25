#
# Makefile
#
# Target: TEST
#
# Environment
#
WORK_INC =  $(WORK_HOME)/inc/
#WORK_INC2 = $(WORK_HOME)/src/PRC_THR/
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
#CCOPTS = -c -I$(WORK_INC) -I$(WORK_INC2)
CCOPTS = -c -I$(WORK_INC)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
#LIBS = -pthread -lSane
LIBS = -pthread -lSane -lYgd
#
#
# Object files
#
#OBJS = msgQRcv.o
OBJS = msgQSnd.o
#
# Target Process
#
TARGET = msgQTEST
#TARGET = msgQRcv
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	#@mv $@ $(WORK_BIN)
	#@rm -rf $(OBJS)
	#@rm -rf core $(TARGET) $(OBJS)

rcv	:
	g++ $(CCOPTS) $(DEFINES) msgQRcv.cpp
	$(LINK) msgQRcv.o $(LIBS) -o msgQRcv

clean :
	rm -rf $(TARGET) $(OBJS) msgQRcv.o msgQRcv
