//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSprcmthr.h"
//------------------------------------------------------------------------------
// CLSprcmthr
//------------------------------------------------------------------------------
CLSprcmthr::CLSprcmthr(void)
//------------------------------------------------------------------------------
// ~CLSprcmthr
//------------------------------------------------------------------------------
CLSprcmthr::~CLSprcmthr(void)
{
	Init();
}
//------------------------------------------------------------------------------
// InitState
//------------------------------------------------------------------------------
void CLSprcmthr::InitState(DB_PRCMTHR *pInfo)
{

}
//------------------------------------------------------------------------------
// CheckState
//------------------------------------------------------------------------------
bool CLSprcmthr::CheckState(void)
{

}
//------------------------------------------------------------------------------
// Delete
//------------------------------------------------------------------------------
void CLSprcmthr::Delete(void)
{
	if (Thread != NULL)
	{
		if (Thread->Active)
		{
			Thread->Stop(true);
		}
		delete Thread;
		Thread = NULL;
	}
	Active = false;
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSprcmthr::Init(void)
{
	ID = -1;
	Thread = NULL;
	Active = false;
}
//------------------------------------------------------------------------------
void CLSprcmthr::Init(DB_PRCMTHR *pInfo)
{
	ID = pInfo->id;
	sprintf(Mng.address, "%s", pInfo->address);
	SetName("PRCMTHR%3d", ID);
	Thread = NULL;
	InitState(pInfo);
	Active = false;
}
//------------------------------------------------------------------------------
// End of CLSprcmthr.cpp
//------------------------------------------------------------------------------