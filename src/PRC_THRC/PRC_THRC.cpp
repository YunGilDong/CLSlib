//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "PRC_THRC.h"
//------------------------------------------------------------------------------

// External Variable
//------------------------------------------------------------------------------
extern void TCLsingHandler(int);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void ClearEnv(void);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
CLSprocess *ShmPrc = NULL;
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
bool Terminate = false;
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
struct timeval TMtimer;

CLSlog Log("PRC_THRC", DIR_LOG);
CLSmap Map("CLMAP");
CLSmemory ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");


void *THRFunc(void *data);

CLSthreadC	Thrt("THRFunc", THRFunc);

void *THRFunc(void *data)
{
	printf("THRFunc\n");
}
//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	Log.Debug("Signal %d accepted", sig);
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
// NeedTerminate
//------------------------------------------------------------------------------
bool NeedTerminate(void)
{
	if (!ShmPrc->IsActiveProcess(getpid()))
	{	
		Log.Write("pid err");
		return (true);
	}
	if (Terminate)
	{
		Log.Write("is terminate");
		return (true);
	}
	if (ShmSys->Terminate)
	{
		Log.Write("system terminate");
		return (true);
	}

	return (false);
}
void ManageThread(void)
{
	//Thrt.Manage();
}
//------------------------------------------------------------------------------
// ManageDebug
//------------------------------------------------------------------------------
void ManageDebug(void)
{
	if (!ShmPrc->ChangeDebug)
		return;

	ShmPrc->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	Log.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	ShmPrc->ChangeDebug = false;
}
bool InitThread(void)
{
	// Start server thread
	/*if (!Thrt.Start())
		return(false);*/

	return(true);
}
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEFAULT_PRC_THRC_LEVEL);
	ManageDebug();
}
bool InitPRCTHRC(void)
{
	CLSprcthrc *pTHRC = ShmPtr->prcthrc;

	// DB Map
	for (int idx = 0; idx < ShmSys->PrcThrC; idx++, pTHRC++)
		Map.AddDB(pTHRC->Mng.address, pTHRC);
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
	ShmPrc = &ShmPtr->process[PRC_THRC];

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

	InitSignal();
	InitOption(argc, argv);

	// 공유메모리 초기화
	if (!InitMemory())
	{
		Log.Write("Shared memory Initialiaztion fail");
		return (false);
	}
	// Thread 초기화
	if (!InitThread())
	{
		Log.Write("Thread initialization fail");
		return (false);
	}
	
	InitDebug();	// Initialize dubgging infomation
	ShmPrc->Register(getpid());
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	ShmPrc->Deregister(getpid());	// Deregister process
	ShmMemory.Detach();				// Detach shared memory
	Log.Write("Process terminate [%d]", getpid());
	exit(0);
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;
	printf("main start\n");
	initOK = InitEnv(argc, argv);
	Log.Write("main start");

	// Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();		// 시작 시각 기록
		ManageDebug();			// Debugging 관리
		//ManageThread();			// Thread 관리

		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(100);			// 100 msec
	}

	ClearEnv();	
	return (0);
}