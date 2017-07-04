//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "MngDatabase.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern SHARED_MEM *ShmPtr;
extern CLSsystem *ShmSys;
//------------------------------------------------------------------------------
// LoadPrcThrC
//------------------------------------------------------------------------------
bool LoadPrcThrC(void)
{
	DB_PRCTHRC info[MAX_PRCTHRC], *pInfo = info;
	CLSprcthrc *pTHRC = ShmPtr->prcthrc;
	ShmSys->PrcThrC = 1;	// temp

	// Set DB (temporary)
	pInfo->id = 1;
	sprintf(pInfo->address, "1.1.1.1");

	// Initialize info
	Log.Debug(1, "PRCTHRC DB:");
	Log.Write("PRCTHRC DB:");
	for (int idx = 0; idx < ShmSys->PrcThrC; idx++, pTHRC++, pInfo++)
	{
		Log.Debug(1, "\t[%d][%d]:[%s]", idx, pInfo->id, pInfo->address);
		Log.Write(1, "\t[%d][%d]:[%s]", idx, pInfo->id, pInfo->address);
		pTHRC->Init(pInfo);
	}
	return (true);
}
//------------------------------------------------------------------------------
// LoadDatabase
//------------------------------------------------------------------------------
bool LoadDatabase(void)
{
	Log.Write("LoadDatabase..");
	// Initialize PrcThrC database
	if (!LoadPrcThrC())
	{
		Log.Write("PrcThrC database initailization fail");
		return (false);
	}

	return (true);	
}