//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSmap.h"

extern CLSlog Log;

//------------------------------------------------------------------------------
// CLSmap
//------------------------------------------------------------------------------
CLSmap::CLSmap(void)
{
}
//------------------------------------------------------------------------------
CLSmap::CLSmap(const char *name) : CLSobject(name)
{
}
//------------------------------------------------------------------------------
// ~CLSmap
//------------------------------------------------------------------------------
CLSmap::~CLSmap(void)
{
}
//------------------------------------------------------------------------------
// Add
//------------------------------------------------------------------------------
bool CLSmap::Add(char *address, CLSequip *ptr)
{
	if (Client.insert(pair<string, CLSequip *>(address, ptr)).second == false)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// AddDB
//------------------------------------------------------------------------------
bool CLSmap::AddDB(char *address, CLSequip *ptr)
{
	if (m_dbase.insert(pair<string, CLSequip *>(address, ptr)).second == false)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// Erase
//------------------------------------------------------------------------------
void CLSmap::Erase(char *address)
{
	MPCL_IT it;

	if ((it = Client.find(address)) == Client.end())
		return;

	Client.erase(it);
}
//------------------------------------------------------------------------------
// Get
//------------------------------------------------------------------------------
CLSequip *CLSmap::Get(char *id)
{
	MPCL_IT it;

	if ((it = Client.find(id)) == Client.end())
		return NULL;

	return (it->second);
}
//------------------------------------------------------------------------------
CLSequip *CLSmap::Get(pthread_t id)
{
	CLSequip *ptr;
	MPCL_IT it;
	CLSthreadC *pThread;

	for (it = Client.begin(); it != Client.end(); it++)
	{
		if ((ptr = it->second) == NULL)
			continue;
		if ((pThread = ptr->Thread) == NULL)
			continue;
		if (pThread->ID == id)
			return (ptr);
	}
	return (NULL);
}
//------------------------------------------------------------------------------
// GetDB
//------------------------------------------------------------------------------
CLSequip *CLSmap::GetDB(char *address)
{
	Log.Write("GetDB 0");
	MPDB_IT it;
	if ((it = m_dbase.find(address)) == m_dbase.end())
	{
		Log.Write("GetDB 000");
		return (NULL);
	}


	Log.Write("GetDB 1");
	return (it->second);
}
//------------------------------------------------------------------------------
// End of CLSmap.cpp
//------------------------------------------------------------------------------