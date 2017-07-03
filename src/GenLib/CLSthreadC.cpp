//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSthreadC.cpp
// Content: pthread
//
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSthreadC.h"
//------------------------------------------------------------------------------
// CLSthread
//------------------------------------------------------------------------------
CLSthreadC::CLSthreadC(void)
{
	//Init();
}
//------------------------------------------------------------------------------
CLSthreadC::CLSthreadC(const char *name) :CLSrunObject(name)
{
	Init();
}
////------------------------------------------------------------------------------
//CLSthreadC::CLSthreadC(const char *name, int interval) : CLSrunObject(name)
//{
//	Init(NULL,NULL,interval);
//}
//------------------------------------------------------------------------------
CLSthreadC::CLSthreadC(const char *name, THR_PTRC startRoutine, void *arg, int interval) :CLSrunObject(name)
{
	Init(startRoutine, arg, interval);
}
//------------------------------------------------------------------------------
// ~CLSthread
//------------------------------------------------------------------------------
CLSthreadC::~CLSthreadC(void)
{
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSthreadC::Init(THR_PTRC startRoutine, void *arg, int interval)
{
	Active = true;
	m_interval = interval;
	m_pArg = arg;
	m_startRoutine = startRoutine;
}
//------------------------------------------------------------------------------
// IsExist
//------------------------------------------------------------------------------
bool CLSthreadC::IsExist(void)
{
	// ID Ȯ��
	if (ID <= 0)
		return (false);

	// Object ���� ���� Ȯ��
	if (pthread_kill(ID, 0) != 0) 
	{
		ID = 0;
		return (false);	
	}

	return (true);
}
//------------------------------------------------------------------------------
// IsRunning
//------------------------------------------------------------------------------
bool CLSthreadC::IsRunning(RUN_STATE *pState)
{
	// Thread ���� ���� Ȯ��
	if (!IsExist())
	{
		*pState = RST_UNEXIST;
		return (false);
	}
	// Thread ���� ���� Ȯ��
	if (CheckElapsedTime(&m_timer, m_interval))
	{
		if (!CheckRunInfo(pState))
		{
			*pState = RST_ABNOMAL;
			return (false);
		}
	}
	*pState = RST_OK;
	return (true);
}
//------------------------------------------------------------------------------
// Kill
//------------------------------------------------------------------------------
void CLSthreadC::Kill(void)
{
	for (int idx = 0; idx < 5; idx++)
	{
		// Thread ���� Ȯ��
		if (!IsExist())
			break;
		// Thread�� �����Ų��.
		if (pthread_kill(ID, SIGUSR1) != 0)
			break;
		usleep(10000); //10 msec
	}
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
void CLSthreadC::Manage(void)
{
	RUN_STATE state;

	// Thread ���� Ȯ��
	if (IsRunning(&state))
	{
		return;
	}
	// Thread ���� ����
	switch (state)
	{
	case RST_ABNOMAL: Kill();
	case RST_UNEXIST: Start(); 
		break;
	default: break;
	}
}
//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
bool CLSthreadC::Start(void *pArg)
{	
	pthread_attr_t	attr;

	// Start Routine ���� ���� Ȯ��
	if (m_startRoutine == NULL)
		return(false);

	// �����带 �����Ѵ�
	if (pthread_attr_init(&attr) != 0)
		return(false);
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
		return(false);

	ResetRunInfo();
	if (pthread_create(&ID, &attr, m_startRoutine, (pArg != NULL) ? pArg : m_pArg) != 0)
		return(false);

	pthread_attr_destroy(&attr);
	gettimeofday(&m_timer, NULL);
	return(true);
}
//------------------------------------------------------------------------------
// Stop
//------------------------------------------------------------------------------
void CLSthreadC::Stop(bool byForce)
{
	Active = false;
	if (byForce)
		pthread_kill(ID, SIGUSR1);
	else pthread_exit(NULL);
}
//------------------------------------------------------------------------------
// End of CLSthread.cpp
//------------------------------------------------------------------------------