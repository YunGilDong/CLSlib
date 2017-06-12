//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "PRC_THR.h"
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
THRtest ThrObj("PRCTHR", DEF_CHECK_INTERVAL);
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
bool Terminate = false;
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
struct timeval TMtimer;

CLSlog Log("PRC_THR", DIR_LOG);
CLSmemory ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");

CLSprcthr *PrcThr = NULL;
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
		return (true);
	if (Terminate)
		return (true);
	if (ShmSys->Terminate)
		return (true);

	return (false);
}
//------------------------------------------------------------------------------
// ManageThread
//------------------------------------------------------------------------------
void ManageThread(void)
{
	ThrObj.manage();
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
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEFAULT_PRC_THR_LEVEL);
	ManageDebug();
}
//------------------------------------------------------------------------------
// InitThread
//------------------------------------------------------------------------------
bool InitThread(void)
{	
	Log.Write("InitThread 1");
	PrcThr->Thread = new THRtest();
	PrcThr->Thread = &ThrObj;
	// Start thread
	if (!ThrObj.start())
		return (false);

	Log.Write("InitThread 2");
	return (true);
}
//------------------------------------------------------------------------------
// TerminateThread
//------------------------------------------------------------------------------
void TerminateThread(void)
{
	ThrObj.Kill();
}
//------------------------------------------------------------------------------
// InitPRCTHR
//------------------------------------------------------------------------------
void InitPRCTHR(void)
{
	Log.Write("InitPRCTHR 1");
	PrcThr = &ShmPtr->prcthr;
	Log.Write("InitPRCTHR 2[%d]", errno);
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
	ShmPrc = &ShmPtr->process[PRC_THR];

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
	InitPRCTHR();

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
	TerminateThread();		// Terminate thread
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

	initOK = InitEnv(argc, argv);
	Log.Write("main start");

	// Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();		// 시작 시각 기록
		ManageDebug();			// Debugging 관리
		ManageThread();			// Thread 관리
		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(100);			// 100 msec
	}
	Log.Write("main end");
	ClearEnv();
	return (0);
}