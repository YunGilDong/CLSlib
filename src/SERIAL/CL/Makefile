#
# Makefile
#
# Target: CLSFILETEST
#
# Environment
#
WORK_INC =  /home/ygd/test/work/GD/inc/
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
CCOPTS = -c -I$(WORK_INC)
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
OBJS = SerialCL.o
#
# Target Process
#
TARGET = SerialCL
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	#@mv CLSFILETEST $(WORK_BIN)
	@rm -rf  $(OBJS)

clean :
	rm -rf $(WORK_BIN) $(TARGET) $(OBJS)
