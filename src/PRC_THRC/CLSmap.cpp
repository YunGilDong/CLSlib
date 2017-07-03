//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLSmap.h"
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
bool CLSmap::Add(char *id, CLSprcthrc *ptr)
{
	if (Client.insert(pair<string, CLSprcthrc *>(id, ptr)).second == false)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// AddDB
//------------------------------------------------------------------------------
bool CLSmap::AddDB(char *address, CLSprcthrc *ptr)
{
	if (m_dbase.insert(pair<string, CLSprcthrc *>(address, ptr)).second == false)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// Erase
//------------------------------------------------------------------------------
void CLSmap::Erase(char *id)
{
	MPCL_IT it;
	if ((it = Client.find(id)) == Client.end())
		return;

	Client.erase(it);
}
//------------------------------------------------------------------------------
// Get
//------------------------------------------------------------------------------
CLSprcthrc *CLSmap::Get(char *id)
{
	MPCL_IT it;

	if ((it = Client.find(id)) == Client.end())
		return (NULL);

	return (it->second);
}
//------------------------------------------------------------------------------
CLSprcthrc *CLSmap::Get(pthread_t id)
{
	CLSprcthrc *ptr;
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
CLSprcthrc *CLSmap::GetDB(char *address)
{
	MPDB_IT it;
	if ((it = m_dbase.find(address)) == m_dbase.end())
		return (NULL);
	
	return (it->second);
}
//------------------------------------------------------------------------------
// End of CLSmap.cpp
//------------------------------------------------------------------------------