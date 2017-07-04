//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSprcthr.h"
//------------------------------------------------------------------------------
// CLSprcthr
//------------------------------------------------------------------------------
CLSprcthr::CLSprcthr(void)
{
	ID = -1;
	Active = false;
}
//------------------------------------------------------------------------------
// ~CLSprcthr
//------------------------------------------------------------------------------
CLSprcthr::~CLSprcthr(void)
{
}
//------------------------------------------------------------------------------
// Delete
//------------------------------------------------------------------------------
void CLSprcthr::Delete(void)
{
	if (Thread != NULL)
	{
		if (Thread->Active)
		{
			Thread->stop();			
		}
		delete Thread;
		Thread = NULL;
	}
	Active = false;
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSprcthr::Init(void)
{
	ID = -1;
	Mng.val = -1;
	Active = false;
	Thread = NULL;
}
//------------------------------------------------------------------------------
void CLSprcthr::Init(DB_PRCTHR *pInfo)
{
	ID = pInfo->id;
	Mng.val = pInfo->val;
	Active = false;
	Thread = NULL;
}
//------------------------------------------------------------------------------
// Update
//------------------------------------------------------------------------------
void CLSprcthr::Update(DB_PRCTHR *pInfo)
{
	Mng.val = pInfo->val;
}