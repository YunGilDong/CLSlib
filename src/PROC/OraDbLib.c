
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[12];
};
static struct sqlcxp sqlfpn =
{
    11,
    "OraDbLib.pc"
};


static unsigned int sqlctx = 156987;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
            short *cud;
   unsigned char  *sqlest;
            char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[4];
   unsigned long  sqhstl[4];
            int   sqhsts[4];
            short *sqindv[4];
            int   sqinds[4];
   unsigned long  sqharm[4];
   unsigned long  *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

/* SQLLIB Prototypes */
extern sqlcxt (/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlcx2t(/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlbuft(/*_ void **, char * _*/);
extern sqlgs2t(/*_ void **, char * _*/);
extern sqlorat(/*_ void **, unsigned int *, void * _*/);

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem(/*_ unsigned char *, signed int * _*/);

 static char *sq0004 = 
"select ISPT_EQUIP_ID ,ISPT_EQUIP_NM ,ISPT_EQUIP_TP ,ISPT_OFFICE_ID  from IS\
PT_EQUIP  order by ISPT_EQUIP_ID            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,4274,846,0,0,
5,0,0,1,0,0,544,23,0,0,0,0,0,1,0,
20,0,0,0,0,0,539,50,0,0,4,4,0,1,0,1,5,0,0,1,5,0,0,1,5,0,0,1,10,0,0,
51,0,0,3,0,0,542,58,0,0,0,0,0,1,0,
66,0,0,4,119,0,521,94,0,0,0,0,0,1,0,
81,0,0,4,0,0,525,97,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,3,0,0,2,5,0,0,
112,0,0,4,0,0,527,106,0,0,0,0,0,1,0,
};


//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "OraDbLib.h"
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
/* EXEC ORACLE OPTION(ORACA = YES); */ 
		// oraca를 사용하기 위함
/* EXEC ORACLE OPTION(RELEASE_CURSOR = YES); */ 
	// Control release of cursors from cursor cache
//------------------------------------------------------------------------------
// DBaseError
//------------------------------------------------------------------------------
void DBaseError(void)
{
	char msgbuf[512];
	size_t msgbuf_len, msg_len;

	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	msgbuf_len = sizeof(msgbuf);
	sqlglm(msgbuf, &msgbuf_len, &msg_len);
	msgbuf[msg_len] = '\0';
	printf("ORA_DB ERROR: [%d] %s", sqlca.sqlcode, msgbuf);
	/* EXEC SQL ROLLBACK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	return;
}
//------------------------------------------------------------------------------
// fnSqlError
//------------------------------------------------------------------------------
intfnSqlError(const char *funcName)
{
	char msg[200];
	size_t bufLen, msgLen;

	bufLen = sizeof(msg);	
	sqlglm(msg, &bufLen, &msgLen);	
#if 1
	printf("[%s] CODE:%d %.*s", funcName, sqlca.sqlcode, msgLen, msg);
#endif
	
	return (sqlca.sqlcode);
}
//------------------------------------------------------------------------------
// DBaseConnect
//------------------------------------------------------------------------------
int DBaseConnect(const char *user, const char *pwd, const char *alias)
{
	oraca.orastxtf = ORASTFERR;	// Only save on SQLERROR (val : 1)
	/* EXEC SQL WHENEVER SQLERROR DO DBaseError(); */ 

	/* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 

	/* EXEC SQL CONNECT :user IDENTIFIED BY :pwd USING :alias; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )10;
 sqlstm.offset = (unsigned int  )20;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)user;
 sqlstm.sqhstl[0] = (unsigned long )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)pwd;
 sqlstm.sqhstl[1] = (unsigned long )0;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)alias;
 sqlstm.sqhstl[2] = (unsigned long )0;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlstm.sqlcmax = (unsigned int )100;
 sqlstm.sqlcmin = (unsigned int )2;
 sqlstm.sqlcincr = (unsigned int )1;
 sqlstm.sqlctimeout = (unsigned int )0;
 sqlstm.sqlcnowait = (unsigned int )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) DBaseError();
}


	return ((sqlca.sqlcode < 0) ? -1 : 0);
}
//------------------------------------------------------------------------------
// DBaseDisconnect
//------------------------------------------------------------------------------ 
int DBaseDisconnect(void)
{
	/* EXEC SQL COMMIT WORK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )51;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) DBaseError();
}


	return ((sqlca.sqlcode < 0)? -1 : 0);
}
//------------------------------------------------------------------------------ 
// DBGenDate
//------------------------------------------------------------------------------ 
void DBGenDate(time_t clock, char *pDate)
{
	struct tm *pTOD;

	pTOD = localtime(&clock);
	sprintf(pDate, "%04d%02d%02d%02d%02d%02d",
		pTOD->tm_year + 1900, pTOD->tm_mon + 1, pTOD->tm_mday, pTOD->tm_hour, pTOD->tm_min, pTOD->tm_sec);
}
//------------------------------------------------------------------------------ 
// DBSelectTest
//------------------------------------------------------------------------------ 
int DBSelectTest(TEST_DB *pInfo, int maxEntry)
{
	printf("DBSelectTest[0]\n");
	int count = 0;
	char *strPtr;
	int i=0;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		TEST_DB *ptr = pInfo;
	/* EXEC SQL END DECLARE SECTION; */ 


	printf("DBSelectTest[1]\n");

	/* EXEC SQL DECLARE csTEST CURSOR FOR
		SELECT ISPT_EQUIP_ID, ISPT_EQUIP_NM, ISPT_EQUIP_TP, ISPT_OFFICE_ID
		FROM ISPT_EQUIP
		ORDER BY ISPT_EQUIP_ID; */ 


	printf("DBSelectTest[2]\n");

	/* EXEC SQL OPEN csTEST; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0004;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )66;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) DBaseError();
}


	for(count = 0; count < maxEntry; ptr++)
	{
		/* EXEC SQL FETCH csTEST INTO :ptr; */ 

{
  struct sqlexd sqlstm;
  sqlorat((void **)0, &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )81;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&ptr->ISPT_EQUIP_ID;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)ptr->ISPT_EQUIP_NM;
  sqlstm.sqhstl[1] = (unsigned long )60;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&ptr->ISPT_EQUIP_TP;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)ptr->ISPT_OFFICE_ID;
  sqlstm.sqhstl[3] = (unsigned long )8;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) DBaseError();
}


		if (sqlca.sqlcode == 1403)
			break;			
		
		printf("ID:[%d] NM:[%.*s] TP:[%d] OFFICE_ID:[%s] sizenm:[%d]\n"
			,ptr->ISPT_EQUIP_ID, strlen(ptr->ISPT_EQUIP_NM), ptr->ISPT_EQUIP_NM, ptr->ISPT_EQUIP_TP, ptr->ISPT_OFFICE_ID, strlen(ptr->ISPT_EQUIP_NM));
		
		count++;
	}
	/* EXEC SQL CLOSE csTEST; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )112;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) DBaseError();
}


	return (count);
}
//------------------------------------------------------------------------------ 
// End of OraDbLib.pc
//------------------------------------------------------------------------------ 