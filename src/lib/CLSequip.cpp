//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSequip.h"
//------------------------------------------------------------------------------
// CLSequip
//------------------------------------------------------------------------------
CLSequip::CLSequip(void)
{
	Init();
}
//------------------------------------------------------------------------------
// ~CLSequip
//------------------------------------------------------------------------------
CLSequip::~CLSequip(void)
{
}
//------------------------------------------------------------------------------
// CheckState
//------------------------------------------------------------------------------
bool CLSequip::CheckState(void)
{

}
//------------------------------------------------------------------------------
bool CLSequip::CheckState(int mode, bool state)
{

}
//------------------------------------------------------------------------------
// Delete
//------------------------------------------------------------------------------
void CLSequip::Delete(void)
{
	if (TcpIF != NULL)
	{
		TcpIF->Close();
		delete TcpIF;
		TcpIF = NULL;
	}
	if (Thread != NULL)
	{
		if (Thread->Active)
			Thread->Stop(true);
		delete Thread;
		Thread = NULL;
	}
	Active = false;
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSequip::Init(void)
{
	ID = -1;
	Socket = -1;
	TcpIF = NULL;
	Thread = NULL;
	Active = false;
}
//------------------------------------------------------------------------------
void CLSequip::Init(DB_EQUIP *pInfo)
{
	ID = pInfo->ISPT_EQUIP_ID;
	Mng.id = pInfo->ISPT_EQUIP_ID;
	Mng.type = pInfo->ISPT_EQUIP_TP;
	sprintf(Mng.address, "%s", pInfo->IP_ADDR);
	SetName("EQUIP%d", ID);
	Socket = -1;
	TcpIF = NULL;
	Thread = NULL;
	InitState(pInfo);
	Active = false;
}
//------------------------------------------------------------------------------
// InitState
//------------------------------------------------------------------------------
void CLSequip::InitState(DB_EQUIP *pInfo)
{
	//
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CLSequip::EquipAuthen(const int devID)
{
	return (true);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CLSequip::EquipLogin(const char *id, const char *pwd)
{
	return (true);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CLSequip::EquipLogout(const char *id)
{
	return (true);
}