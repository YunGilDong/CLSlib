#
# Makefile
#
# Target : OraDbLib
#
# Environment
#
WORK_INC = $(WORK_HOME)/inc/
WORK_LIB = $(WORK_HOME)/lib/
WORK_LIB2 = /home/ygd/test/work/GD/CLSlib/src/PROC
ORA_INC = $(ORACLE_INC)
#ORA_LIB = $(ORACLE_HOME)/lib
ORA_LIB = /usr/lib/oracle/11.2/client64/lib
#
# Rules
#
DEFINES =
PCDEFINES = define=UNSUPPORT_BOOL define=HP_UX
CCOPTS = -c $(DEFINES) -I$(WORK_INC) -I$(ORA_INC) -L$(ORA_LIB) -lclntsh
PCOPTS = $(PCDEFINES) include=$(WORK_INC) include=$(ORACLE_INC) include=$(ORA_LIB)
LDOPTS = $(DEFINES) -L$(WORK_LIB2) -L$(ORA_LIB)
AROPTS = rcv
CC = gcc $(CCOPTS)
CCP = g++ $(CCOPTS)
PC = proc $(PCOPTS)
AR = ar $(AROPTS)
LINK = gcc $(LDOPTS)

.c.o:	; $(CC) $<
#
# Libraries
#
LIBS = -lOraDb -lclntsh
#
# Library files used for dependency tests
#
LIB_DEPS =
#
# Objects
#
OBJS = OraDbLib.o
#
# Target
#
TARGET = libOraDb.a
#
# Make
#
#default:	$(TARGET)
all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(AR) $@ $(OBJS)
	#@mv $@ $(WORK_LIB)
	#@rm -fr core $(TARGET) $(OBJS) tp*

OraDbLib.c:	OraDbLib.pc
	$(PC) mode=oracle dbms=v8 unsafe_null=yes char_map=string iname=OraDbLib.pc oname=OraDbLib.c 
	#$(PC) dbms=v8 unsafe_null=yes char_map=string code=cpp parse=partial iname=OraDbLib.pc oname=OraDbLib.c

main:
	@gcc -c OraDbMain.c -I$(ORA_INC)
	#$(CC) OraDbMain.c -L$(ORA_LIB) -o OraDbMain
	#@gcc -L$(ORA_LIB) -L$(WORK_LIB2) OraDbMain.o $(LIBS) -o OraDbMain
	$(LINK) OraDbMain.o $(LIBS) -o OraDbMain

mclean:
	@rm -fr core OraDbMain.o OraDbMain

clean:
	@rm -fr core *.lis *.cud *.dcl *.cod tp* $(TARGET) $(OBJS) OraDbLib.c
#
# End of Makefile
#
