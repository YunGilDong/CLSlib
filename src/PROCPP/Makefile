#
# Makefile
#
# Target : libOraDbCpp
#
# Environment
#
WORK_INC = $(WORK_HOME)/inc/
WORK_LIB = $(WORK_HOME)/lib/
WORK_LIB2 = /home/ygd/test/work/GD/CLSlib/src/PROCPP
ORA_INC = $(ORACLE_INC)
#ORA_LIB = $(ORACLE_HOME)/lib
ORA_DBMS = $(ORACLE_HOME)/rdbms/public
ORA_LIB = /usr/lib/oracle/11.2/client64/lib
#
# Rules
#
DEFINES =
PCDEFINES = define=UNSUPPORT_BOOL define=HP_UX
CCOPTS = -c $(DEFINES) -I$(WORK_INC) -I$(ORA_INC) -L$(ORA_LIB)
PCOPTS = $(PCDEFINES) include=$(WORK_INC) include=$(ORACLE_INC) include=$(ORA_DBMS) include=$(ORA_LIB)
LDOPTS = $(DEFINES) -L$(WORK_LIB2) -L$(ORA_LIB)
AROPTS = rcv

AR = ar $(AROPTS)
CC = g++ $(CCOPTS)
PC = proc $(PCOPTS)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries
#
LIBS = -lOraDbCpp -lclntsh
#
# Library files used for dependency tests
#
LIB_DEPS =
#
# Objects
#
OBJS = OraDbCppLib.o
#
# Target
#
TARGET = libOraDbCpp.a
#
# Make the executable program
#
default:	$(TARGET)

$(TARGET):	$(OBJS)
	$(AR) $@ $(OBJS)
	@mv $@ $(WORK_LIB)
	@rm -fr core $(TARGET) $(OBJS) tp*

OraDbCppLib.cpp:	OraDbCppLib.pc
	#$(PC) mode=oracle dbms=v8 unsafe_null=yes char_map=string iname=OraDbCppLib.pc oname=OraDbCppLib.cpp
	$(PC) dbms=v8 unsafe_null=yes char_map=string CODE=CPP parse=partial iname=OraDbCppLib.pc oname=OraDbCppLib.cpp
	#$(PC) dbms=v8 unsafe_null=yes char_map=string code=ansi_c parse=partial iname=OraDbCppLib.pc oname=OraDbCppLib.cpp
	#$(PC) dbms=v8 unsafe_null=yes char_map=string code=ansi_c parse=full iname=OraDbLib.pc oname=OraDbLib.c

main:
	@g++ -c OraDbCppMain.cpp -I$(ORA_INC) -I$(WORK_INC) -I$(WORK_LIB2)
	$(LINK) OraDbCppMain.o $(LIBS) -o OraDbCppMain

clean:
	@rm -fr core $(WORK_LIB)$(TARGET) *.lis *.cud *.dcl *.cod tp* $(TARGET) $(OBJS) OraDbCppLib.cpp OraDbCppMain.o OraDbCppMain
#
# End of Makefile
#
