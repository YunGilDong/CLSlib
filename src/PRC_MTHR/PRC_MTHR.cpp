//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "PRC_MTHR.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern void *THRserver(void *);
extern void TSVsigHandler(int);
extern void TCLsigHandler(CLSprcmthr *, int);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void ClearEnv(void);
//------------------------------------------------------------------------------
// Glabal Variable
//------------------------------------------------------------------------------
CLSlog Log("PRC_MTHR", DIR_LOG);
CLSmap Map("CLMAP");
//------------------------------------------------------------------------------
// Local Variable
bool Terminate = false;
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
CLSprocess *ShmPrc = NULL;
struct timeval TMtimer;

CLSmemory ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
CLSthreadC ThrServer("MTHRSV", THRserver);
//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	Log.Write(1, "Signal %d accepted", sig);
	switch (sig)
	{
	case SIGTERM:	// 15
	case SIGKILL:	// 9
		ClearEnv();
		break;
	default:	break;
	}
}
//------------------------------------------------------------------------------
// THRsigHandler
//------------------------------------------------------------------------------
void THRsigHandler(int sig)
{
	pthread_t id = pthread_self();
	CLSprcmthr *ptr;

	// Process server thread signal
	if (id == ThrServer.ID)
	{
		TSVsigHandler(sig);
		return;
	}
	// Process Client thread signal
	if ((ptr = Map.Get(id)) != NULL)
		TCLsigHandler(ptr, sig);
}
//------------------------------------------------------------------------------
// NeedTerminate
//------------------------------------------------------------------------------
bool NeedTerminate(void)
{
	if (Terminate)
		return (true);
	if (ShmSys->Terminate)
		return (true);

	return (false);
}
//------------------------------------------------------------------------------
// DeleteClient
//------------------------------------------------------------------------------
void DeleteClient(CLSprcmthr *pMTHR, char *address)
{
	// Client 종료 요구
	if (pMTHR->Thread != NULL)
		pMTHR->Thread->Terminate = true;
	// 종료 대기
	for (int idx = 0; idx < 5 && pMTHR->Thread->Active; idx++)
		usleep(5000);
	pMTHR->Delete();
	Map.Erase(address);
}
//------------------------------------------------------------------------------
// DeleteClientX
//------------------------------------------------------------------------------
void DeleteClientX(CLSprcmthr *pMTHR)
{
	pMTHR->Delete();
}
//------------------------------------------------------------------------------
// ManageState
//------------------------------------------------------------------------------
void ManageState(void)
{
	//
}
//------------------------------------------------------------------------------
// ManageThread
//------------------------------------------------------------------------------
void ManageThread(void)
{
	CLSprcmthr *ptr;
	MPCL_IT it;
	RUN_STATE state;
	CLSthreadC *pThread;

	// Manage server thread
	ThrServer.Manage();
	/*
	// Manage client thread
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		// Thread 실행 상태 확인
		if ((ptr = it->second) == NULL)
		{
			Log.Write(1, "Map state abnomal [1]");
			Map.Client.erase(it++);
			continue;
		}
		pThread = ptr->Thread;
		if (pThread != NULL && pThread->IsRunning(&state))
			++it;
		else
		{
			Log.Write(1, "Map state abnomal [2]");
			DeleteClientX(ptr);
			Map.Client.erase(it++);
		}
	}
	*/
}
//------------------------------------------------------------------------------
// ManageTest
//------------------------------------------------------------------------------
void ManageTest(void)
{
	//
}
//------------------------------------------------------------------------------
// ManageDebug
//------------------------------------------------------------------------------
void ManageDebug(void)
{
	MPCL_IT it;
	CLSprcmthr *ptr;

	if (!ShmPrc->ChangeDebug)
		return;

	ShmPrc->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	Log.SetDebug(ShmPrc->Level, ShmPrc->Target);
	ThrServer.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	for (it = Map.Client.begin(); it != Map.Client.end(); it++)
	{
		if ((ptr = it->second) == NULL)
			continue;
	}
	ShmPrc->ChangeDebug = false;
}
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEFAULT_PRC_MTHR_LEVEL);
	ManageDebug();
}
//------------------------------------------------------------------------------
// InitThread
//------------------------------------------------------------------------------
bool InitThread(void)
{
	// Start server thread
	if (!ThrServer.Start())
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// TerminateThread
//------------------------------------------------------------------------------
void TerminateThread(void)
{
	CLSprcmthr *ptr;
	CLSthreadC *pThread;
	MAP_CLIENT::iterator it;

	Log.Write(1, "TerminateThread..[1]");
	// Terminate client thread
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		if ((ptr = it->second) == NULL)
			++it;
		else if ((pThread = ptr->Thread) == NULL)
			++it;
		else if (pThread->IsExist())
		{
			pThread->Kill();
			Log.Write("#Client thread kill#");
		}
		else
		{
			delete pThread;
			ptr->Thread = NULL;
			++it;
		}
	}
	Log.Write(1, "TerminateThread[2]");
	// Terminate server thread
	ThrServer.Kill();
}
//------------------------------------------------------------------------------
// InitMTHR
//------------------------------------------------------------------------------
bool InitMTHR(void)
{
	CLSprcmthr *pMTHR = ShmPtr->prcmthr;

	// DB Map
	for (int idx = 0; idx < ShmSys->Prcmthr; idx++, pMTHR++)
		Map.AddDB(pMTHR->Mng.address, pMTHR);

	// Test
	MPCL_IT it;
	CLSprcmthr *ptr;
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		ptr = it->second;
		printf("ptrID:%d\n", ptr->ID);
	}
	Log.Write(1, "InitMTHR sucess");
}
//------------------------------------------------------------------------------
// InitOption
//------------------------------------------------------------------------------
void InitOption(int argc, char **argv)
{
}
//------------------------------------------------------------------------------
// InitMemory
//------------------------------------------------------------------------------
bool InitMemory(void)
{
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Attach()) == (void *)-1)
		return (false);

	ShmSys = &ShmPtr->system;
	ShmPrc = &ShmPtr->process[4];
	return (true);
}
//------------------------------------------------------------------------------
// InitSignal
//------------------------------------------------------------------------------
void InitSignal(void)
{
	signal(SIGQUIT, SigHandler);
	signal(SIGINT, SigHandler);
	signal(SIGTERM, SigHandler);
	signal(SIGKILL, SigHandler);
	signal(SIGABRT, SigHandler);
	signal(SIGCLD, SigHandler);
	signal(SIGPIPE, SigHandler);
	signal(SIGCHLD, SigHandler);
	// Deamon 작업 처리
	setpgrp();
	signal(SIGHUP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
}
//------------------------------------------------------------------------------
// InitEnv
//------------------------------------------------------------------------------
bool InitEnv(int argc, char **argv)
{
	Log.Write("Process start [%d]", getpid());

	InitSignal();		// 시그널 처리기 초기화
	InitOption(argc, argv);		// 옵션 처리

	// 공유메모리 초기화
	if (!InitMemory())
	{
		Log.Write("Shared memory initialization fail");
		return (false);
	}
	// Thread 초기화
	if (!InitThread())
	{
		Log.Write("Thread initialzation fail");
		return (false);
	}

	InitMTHR();			// MTHR 정보 초기화
	InitDebug();		// Initialize debugging infomation
	ShmPrc->Register(getpid());		// Register process
	gettimeofday(&TMtimer, NULL);
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	Log.Write(1, "ClearEnv..[1]");
	TerminateThread();			// Terminate thread
	ShmPrc->Deregister(getpid());	// Deregister process
	ShmMemory.Detach();				// Detach shared memory
	Log.Write(1, "ClearEnv[2]");
	Log.Write("Process terminate [%d]", getpid());	
	exit(0);
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;

	
	// 작업 환경 초기화
	initOK = InitEnv(argc, argv);
	printf("prcmthr main start\n");
	// Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();		// 시작 시각 기록
		ManageDebug();			// 디버깅 관리
		ManageThread();			// Thread 관리

		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(100);		// 100 msec
	}
	// 작업 환경 정리
	ClearEnv();
	
	printf("prcmthr main end\n");
	return (0);
	
}
//------------------------------------------------------------------------------
// End of PRC_MTHR.cpp
//------------------------------------------------------------------------------