//------------------------------------------------------------------------------
#ifndef OraDbCppLibH
#define OraDbCppLibH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "Database.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern int DBaseConnect_cpp(char *user, char *pwd, char *alias);
extern int DBaseDisconnect_cpp(void);
//------------------------------------------------------------------------------
extern int DBSelectTest_cpp(YGD_DB *pInfo, int maxEntry);
//------------------------------------------------------------------------------
//extern int DBSelectTest_cpp(TEST_DB *pInfo, int maxEntry);
//extern int DBGetEquip_cpp(DB_EQUIP *pInfo, int maxEntry);
//------------------------------------------------------------------------------
#endif // !OraDbCppLibH
//------------------------------------------------------------------------------
// End of OraDbCppLib.h
//------------------------------------------------------------------------------

