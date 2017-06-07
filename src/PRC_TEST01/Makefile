#
# Makefile
#
# Target: CLSFILETEST
#
# Environment
#
WORK_INC =  /home/ygd/test/work/GD/inc/
WORK_INC2 = /home/ygd/test/work/GD/src/PRC_THR/
WORK_LIB =  /home/ygd/test/work/GD/lib/
WORK_BIN =  /home/ygd/test/work/GD/bin/
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
LIBS = -lpthread -lSane
#
#
# Object files
#
OBJS = PRC_TEST01.o
#
# Target Process
#
TARGET = PRC_TEST01
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	@mv $@ $(WORK_BIN)
	#@mv CLSFILETEST $(WORK_BIN)
	#@rm -rf $(OBJS)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_BIN)$(TARGET) $(OBJS)
