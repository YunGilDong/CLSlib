#
# Makefile
#
# Target: CLSMAIN
#
# Environment
#
WORK_INC =  $(WORK_HOME)/inc/
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
CCOPTS = -c -I$(WORK_INC)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
LIBS = -pthread -lYgd -lSane
#
#
# Object files
#
OBJS = CLSMAIN.o MngDatabase.o
#
# Target Process
#
TARGET = CLSMAIN
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	#@mv CLSFILETEST $(WORK_BIN)
	@mv $@ $(WORK_BIN)
	#@rm -rf  $(OBJS)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_BIN)$(TARGET) $(TARGET) $(OBJS)
