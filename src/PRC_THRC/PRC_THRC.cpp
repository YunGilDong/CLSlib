//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "PRC_THRC.h"
//------------------------------------------------------------------------------

// External Variable
//------------------------------------------------------------------------------
extern void *THRclient(void *);
extern void TCLsigHandler(int);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void ClearEnv(void);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
CLSlog Log("PRC_THRC", DIR_LOG);
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
CLSthreadC	ThrClient("PRCTHRC", THRclient);
//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	Log.Debug("Signal %d accepted", sig);
	Log.Write("Signal %d accepted", sig);
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

	// Processs thread signal
	if (id != ThrClient.ID)
		return;

	TCLsigHandler(sig);
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
//------------------------------------------------------------------------------
// DeleteClientX
//------------------------------------------------------------------------------
void DeleteClientX(CLSprcthrc *pTHRC)
{
	pTHRC->Delete();
}
//------------------------------------------------------------------------------
// ManageThread
//------------------------------------------------------------------------------
void ManageThread(void)
{
	// THRclient.Manage();
	CLSprcthrc *ptr;
	MPCL_IT it;
	RUN_STATE state;
	CLSthreadC *pThread;

	// Manage thread
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		// Thread 실행 상태 확인
		if ((ptr = it->second) == NULL)
		{
			Log.Write("Map state abnormal [1]");
			Map.Client.erase(it++);
			continue;
		}
		//printf("ID:%d\n", ptr->ID);
		pThread = ptr->Thread;
		pThread->Manage();		// Manage thread
		if (pThread != NULL && pThread->IsRunning(&state))
			++it;
		/*else
		{
			Log.Write("Map state abnormal [2]");
			DeleteClientX(ptr);
			Map.Client.erase(it++);
		}*/
	}
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
	char name[SHORTBUF_LEN];
	CLSprcthrc *dPtr;
	Log.Write("InitThread [0]");
	// 등록된 OBJ인제 확인
	if ((dPtr = Map.GetDB("1.1.1.1")) == NULL)
	{		
		Log.Write("GetDB:[%s][%d]", dPtr->Mng.address, dPtr->ID);
		Log.Write("Undefined Obj access [%s]", MAP_KEY);
		return (false);
	}
	Log.Write("InitThread [1]");
	Log.Write("GetDB:[%s][%d]", dPtr->Mng.address, dPtr->ID);
	sprintf(name, "CL%04d", dPtr->ID);
	if ((dPtr->Thread = new CLSthreadC(name, THRclient, dPtr)) == NULL)
	{
		Log.Write("Thread create fail [%s]", name);
		return (false);
	}
	Log.Write("InitThread [2]");
	// Thread 관리를 위한 map 생성
	if (!Map.Add(dPtr->Mng.address, dPtr))
	{
		Log.Write("Client add fail [%s]", name);
		return (false);
	}
	Log.Write("InitThread [3]");
	// Thread start
	if (!dPtr->Thread->Start())
	{
		Log.Write("Thread start fail [%s]", name);
		return (false);
	}
	Log.Write("InitThread [4]");
	dPtr->Active = true;
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
//------------------------------------------------------------------------------
// InitPRCTHRC
//------------------------------------------------------------------------------
bool InitPRCTHRC(void)
{
	CLSprcthrc *pTHRC = ShmPtr->prcthrc;

	// DB Map
	for (int idx = 0; idx < ShmSys->PrcThrC; idx++, pTHRC++)
		Map.AddDB(pTHRC->Mng.address, pTHRC);


	MPCL_IT it;
	CLSprcthrc *ptr;
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		ptr = it->second;
		printf("ptr ID : %d\n", ptr->ID);
	}
	Log.Write("InitPRCTHRC success");
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
	Log.Write("Init Signal.");
}
//------------------------------------------------------------------------------
// InitEnv
//------------------------------------------------------------------------------
bool InitEnv(int argc, char **argv)
{
	Log.Write("Process start [%d]", getpid());

	InitSignal();
	InitOption(argc, argv);

	Log.Write("Init start");

	// 공유메모리 초기화
	if (!InitMemory())
	{
		Log.Write("Shared memory Initialiaztion fail");
		return (false);
	}
	InitPRCTHRC();	// THRC 정보 초기화
	// Thread 초기화
	if (!InitThread())
	{
		Log.Write("Thread initialization fail");
		return (false);
	}
	Log.Write("InitEnv success");	
	InitDebug();	// Initialize dubgging infomation
	ShmPrc->Register(getpid());
	gettimeofday(&TMtimer, NULL);
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
	printf("main start[PRC_THRC]\n");
	initOK = InitEnv(argc, argv);	

	// Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();		// 시작 시각 기록
		ManageDebug();			// Debugging 관리
		ManageThread();			// Thread 관리

		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(100);			// 100 msec
	}

	ClearEnv();	
	return (0);
}