//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "THRserver.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern SHARED_MEM *ShmPtr;
extern CLSthreadC ThrServer;

extern void DeleteClient(CLSequip *ptr, int id);
extern void *THRclient(void *);
extern void THRsigHandler(int);
extern bool NeedTerminate(void);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void TSVclearEnv(void);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
// Local Variable
int CallCnt = 1;
//------------------------------------------------------------------------------
// CLStcp TcpServer("EQUIPSVTCP","TCP_SERVER_PORT, "")'

//------------------------------------------------------------------------------
// TSVsigHandler
//------------------------------------------------------------------------------
void TSVsigHandler(int sig)
{
	Log.Write(1, "Server signal %d accepted", sig);
	switch (sig)
	{
	case SIGUSR1:	// 10
	case SIGSEGV:	// 11
	case SIGPIPE:	// 13
		TSVclearEnv();
		break;
	default:	break;
	}
}
//------------------------------------------------------------------------------
// TSVcreateClient
//------------------------------------------------------------------------------
bool TSVcreateClient(int id)
{
	char name[SHORTBUF_LEN];
	CLSequip *cPtr, *dPtr;

	Log.Write("TSVcreateClient [0]");
	// 등록된 Equip 인지 확인
	if ((dPtr = Map.GetDB(id)) == NULL)
	{
		Log.Write(1, "Undefined Equip ID access [%s]", id);
		return (false);
	}
	Log.Write("TSVcreateClient [1]");
	// 존재하는 Client확인
	sprintf(name, "CL%d", dPtr->ID);
	if ((cPtr = Map.Get(id)) != NULL)
	{
		Log.Write(1, "Client already exist [%s]", name);
		DeleteClient(cPtr, id);
	}
	Log.Write("TSVcreateClient [2]");
	// Client thread create
	if ((dPtr->Thread = new CLSthreadC(name, THRclient, dPtr)) == NULL)
	{
		Log.Write(1, "Client create fail [%s]", name);
		delete dPtr->Thread;
		return (false);
	}
	Log.Write("TSVcreateClient [3]");
	// Thread 관리를 위해 map 생성
	if (!Map.Add(id, dPtr))
	{
		Log.Write(1, "Client add fail [%s]", name);
		return (false);
	}
	Log.Write("TSVcreateClient [4]");
	// Child thread start
	Log.Write(1, "New client added [%s]", name);
	if (!dPtr->Thread->Start())
	{
		Log.Write(1, "Client start fail [%s]", name);
		return (false);
	}
	dPtr->Active = true;
	Log.Write("TSVcreateClient [5]finish");
	return (true);
}
//------------------------------------------------------------------------------
// TSVmanage
//------------------------------------------------------------------------------
bool TSVmanage(void)
{
	int id1, id2, id3, id4, id5;
	id1 = 1000;	id2 = 1001; id3 = 1002; id4 = 1003; id5 = 1234;

	if (CallCnt == 1)
	{
		if (TSVcreateClient(id1))
		{
			CallCnt++;
			return (true);
		}
	}
	if (CallCnt == 2)
	{
		if (TSVcreateClient(id2))
		{
			CallCnt++;
			return (true);
		}
	}
	if (CallCnt == 3)
	{
		if (TSVcreateClient(id3))
		{
			CallCnt++;
			return (true);
		}
	}
	if (CallCnt == 4)
	{
		if (TSVcreateClient(id4))
		{
			CallCnt++;
			return (true);
		}
	}
	if (CallCnt == 5)
	{
		if (TSVcreateClient(id5))
		{
			CallCnt++;
			return (true);
		}
	}
	return (true);
}
//------------------------------------------------------------------------------
// TSVinitNetwork
//------------------------------------------------------------------------------
bool TSVinitNetwork(void)
{
	return (true);
}
//------------------------------------------------------------------------------
// TSVinitSignal
//------------------------------------------------------------------------------
void TSVinitSignal(void)
{
	sigset_t newMask;
	struct sigaction act;

	// Signal 처리기 초기화
	sigemptyset(&newMask);
	sigaddset(&newMask, SIGSEGV);
	sigaddset(&newMask, SIGHUP);
	sigaddset(&newMask, SIGCLD);
	sigaddset(&newMask, SIGCHLD);
	sigaddset(&newMask, SIGPIPE);
	sigaddset(&newMask, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &newMask, NULL);	// thread signal 초기화, 위에서 초기화한 시그널만 프로세스에서 받아들임
	act.sa_handler = THRsigHandler;
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGCLD, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
}
//------------------------------------------------------------------------------
// TSVinitEnv
//------------------------------------------------------------------------------
bool TSVinitEnv(void)
{
	// Signal 처리기 초기화
	pthread_detach(pthread_self());
	TSVinitSignal();

	// Network초기화
	if (!TSVinitNetwork())
		return (false);

	return true;
}
//------------------------------------------------------------------------------
// TSVclearEnv
//------------------------------------------------------------------------------
void TSVclearEnv(void)
{
	ThrServer.Stop();	// Terminate thread
}
//------------------------------------------------------------------------------
// THRserver
//------------------------------------------------------------------------------
void *THRserver(void *data)
{
	bool initOK;
	initOK = TSVinitEnv();	// 작업 환경 초기화

	Log.Write("THRserver log address %d ", Log);

	// Main loop
	while (initOK && !ThrServer.Terminate && !NeedTerminate())
	{
		//Log.Debug("THRserver run##"); 
		ThrServer.MarkTime();

		//if (!TSVmanage())
		//	break;

		ThrServer.UpdateRunInfo();	// 실행 정보 갱신
		ThrServer.Pause(1000);		// 10 msec
	}
	//TCLclearEnv();		// 작업 환경 정리
}