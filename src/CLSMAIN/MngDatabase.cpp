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
//
//------------------------------------------------------------------------------
bool LoadPrcmthr(void)
{
	DB_PRCMTHR info[MAX_PRCMTHR], *pInfo = info;
	CLSprcmthr *pMTHR = ShmPtr->prcmthr;
	ShmSys->Prcmthr = 3;	// temp ( prcmthr count)

	// Set DB (temporary)
	int id = 10;
	
	// Initialize info
	for (int idx = 0; idx < ShmSys->Prcmthr; idx++, pMTHR++, pInfo++, id++)
	{
		if (id == 10)
			sprintf(pInfo->address, "10.10.10.10");
		else if (id == 11)
			sprintf(pInfo->address, "11.11.11.11");
		else if (id == 12)
			sprintf(pInfo->address, "12.12.12.12");

		Log.Debug(1, "\t[%d][%d]:[%s]", idx, pInfo->id, pInfo->address);
		Log.Write(1, "\t[%d][%d]:[%s]", idx, pInfo->id, pInfo->address);
		pMTHR->Init(pInfo);
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

	// Initialize Prcmthr database
	if (!LoadPrcmthr())
	{
		Log.Write("Prcmthr database initialization fail");
		return (false);
	}

	return (true);	
}