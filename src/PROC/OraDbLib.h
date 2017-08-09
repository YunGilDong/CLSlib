//------------------------------------------------------------------------------
#ifndef _ORADBLIBH
#define _ORADBLIBH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sqlca.h>
#include <oraca.h>
#include <sqlcpr.h>
#include <sqlda.h>
#include <oci.h>
//------------------------------------------------------------------------------
// External Definition
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

	int DBaseConnect(const char *user, const char *pwd, const char *alias);
	int DBaseDisconnect(void);
	
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _ORADBLIBH
//------------------------------------------------------------------------------
// End of OraDbLib.h
//------------------------------------------------------------------------------
