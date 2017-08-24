//------------------------------------------------------------------------------
#ifndef _ORADBLIBH
#define _ORADBLIBH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "General.h"
#include "Define.h"
#include "Database.h"
//------------------------------------------------------------------------------
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <oraca.h>
#include <sqlcpr.h>
#include <sqlda.h>
#include <oci.h>
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// External Definition
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	extern int DBaseConnect(const char *user, const char *pwd, const char *alias);
	extern int DBaseDisconnect(void);
	extern int DBSelectTest(TEST_DB *pInfo, int maxEntry);
	extern int DBGetEquip(DB_EQUIP *pInfo, int maxEntry);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _ORADBLIBH
//------------------------------------------------------------------------------
// End of OraDbLib.h
//------------------------------------------------------------------------------
