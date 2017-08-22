//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CPVIMS.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern void *THRserver(void *);
extern void TSVsigHandler(int);
extern void TCLsigHandler(CLSequip *, int);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void ClearEnv(void);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
CLSlog Log("CPVIMS", DIR_LOG);
CLSmap Map("CLMAP"); 
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
bool Terminate = false;
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
CLSprocess *ShmPrc = NULL;
struct timeval TMtimer;

CLSmemory ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
CLSthreadC ThrServer("CPVIMSSV", THRserver);
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
	CLSequip *ptr;

	Log.Write(1, "Server Signal %d accepted", sig);

	// Processserver thread signal
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
void DeleteClient(CLSequip *pEquip, int id)
{
	// Client 종료 요구
	if (pEquip->Thread != NULL)
		pEquip->Thread->Terminate = true;
	// 종료 대기
	for (int idx = 0; idx < 5 && pEquip->Thread->Active; idx++)
		usleep(5000);	// 5ms
	pEquip->Delete();
	Map.Erase(id);
} 
//------------------------------------------------------------------------------
// DeleteClientX
//------------------------------------------------------------------------------
void DeleteClientX(CLSequip *pEquip)
{
	pEquip->Delete();
}
//------------------------------------------------------------------------------
// ManageState
//------------------------------------------------------------------------------
void ManageState(void)
{

}
//------------------------------------------------------------------------------
// ManageThread
//------------------------------------------------------------------------------
void ManageThread(void)
{
	CLSequip *ptr;
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
			Log.Write(1, "Map State abnomal [1]");
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

}
//------------------------------------------------------------------------------
// ManageDebug
//------------------------------------------------------------------------------
void ManageDebug(void)
{
	MPCL_IT it;
	CLSequip *ptr;

	if (!ShmPrc->ChangeDebug)
		return;

	ShmPrc->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	Log.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	ThrServer.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	for (it = Map.Client.begin(); it != Map.Client.end(); it++)
	{
		if ((ptr = it->second) == NULL)
			continue;
		//if (ptr->TcpIF == NULL)
		//	continue;

		//ptr->TcpIF->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	}
	ShmPrc->ChangeDebug = false;
}
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEFAULT_CPVIMS_LEVEL);
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
	CLSequip *ptr;
	CLSthreadC *pThread;
	MAP_CLIENT::iterator it;

	// Terminate client thread
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		if ((ptr = it->second) == NULL)
			++it;
		else if ((pThread = ptr->Thread) == NULL)
			++it;
		else if (pThread->IsExist())
			pThread->Kill();
		else
		{
			delete pThread;
			ptr->Thread = NULL;
			++it;
		}
	}
	// Terminate server thread
	ThrServer.Kill();
}
//------------------------------------------------------------------------------
// InitEquip
//------------------------------------------------------------------------------
bool InitEquip(void)
{
	CLSequip *pEquip = ShmPtr->equip;

	// DB Map
	for (int idx = 0; idx < ShmSys->Equip; idx++, pEquip++)
		Map.AddDB(pEquip->Mng.id, pEquip);
}
//------------------------------------------------------------------------------
// InitOption
//------------------------------------------------------------------------------
void InitOption(int argc, char ** argv)
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
	ShmPrc = &ShmPtr->process[PRC_CPVIMS];
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
	// Daemon 작업 처리
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

	InitSignal();	// 시그널 처리기 초기화
	Log.Write("Init Signal");
	InitOption(argc, argv);		// 옵션 처리

	// 공유메모리 초기화
	if (!InitMemory())
	{
		Log.Write("Shared memory initialization fail");
		return (false);
	}
	Log.Write("Init Memory");
	// Thread 초기화
	if (!InitThread())
	{
		Log.Write("Thread initialization fail");
		return (false);
	}
	Log.Write("Init Thread");
	
	InitEquip();		// Equip 정보 초기화
	InitDebug();		// Initialize debugging information	
	ShmPrc->Register(getpid());	// Register process
	gettimeofday(&TMtimer, NULL);
	Log.Write("Init Env");
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	TerminateThread();		// Terminate thread
	ShmPrc->Deregister(getpid());		// Deregister process
	ShmMemory.Detach();		// Detach shared memory
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

	//Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();		// 시작 시작 기록
		ManageDebug();			// 디버깅 관리
		ManageThread();			// Thread 관리
		
		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(100);			// 100 msec
	}
}