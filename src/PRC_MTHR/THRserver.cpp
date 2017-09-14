//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "THRserver.h"
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern SHARED_MEM *ShmPtr;
extern CLSthreadC ThrServer;

extern void DeleteClient(CLSprcmthr *ptr, char *name);
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
//------------------------------------------------------------------------------
int CallCnt = 1;
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
bool TSVcreateClient(char *remoteIP)
{
	char name[SHORTBUF_LEN];
	CLSprcmthr *cPtr, *dPtr;

	// 등록된 MTHR인가 확인
	if ((dPtr = Map.GetDB(remoteIP)) == NULL)
	{
		Log.Write(1, "Undefined MTHR access [%s]", remoteIP);
		return (false);
	}
	// 존재하는 Client 확인
	sprintf(name, "MTHR%4d", dPtr->ID);
	if ((cPtr = Map.Get(remoteIP)) != NULL)
	{
		Log.Write("Client already exist [%s]", name);
		DeleteClient(cPtr, remoteIP);
		return (false);
	}
	if ((dPtr->Thread = new CLSthreadC(name, THRclient, dPtr)) == NULL)
	{
		Log.Write("Client create fail [%s]", name);
		DeleteClient(dPtr, remoteIP);
		return (false);
	}
	// Thread 관리를 위한 Map 생성 (Client등록)
	if (!Map.Add(remoteIP, dPtr))
	{
		Log.Write("Client add fail[%s]", name);
		return (false);
	}
	Log.Write(1, "New client added [%s:%s]", name, remoteIP);
	// Child thread start
	if (!dPtr->Thread->Start())
	{
		Log.Write(1, "Client start fail [%s]", name);
		return (false);
	}
	dPtr->Active = true;
	return (true);
}
//------------------------------------------------------------------------------
// TSVmanage
//------------------------------------------------------------------------------
bool TSVmanage(void)
{
	char ip1[SHORTBUF_LEN];
	char ip2[SHORTBUF_LEN];
	char ip3[SHORTBUF_LEN];

	sprintf(ip1, "10.10.10.10");
	sprintf(ip2, "11.11.11.11");
	sprintf(ip3, "12.12.12.12");

	if (CallCnt == 1)
	{
		//if (TSVcreateClient("10.10.10.10"))
		if (TSVcreateClient(ip1))
		{
			CallCnt++;
			return (true);
		}
	}
	if (CallCnt == 2)
	{
		//if (TSVcreateClient("11.11.11.11"))
		if (TSVcreateClient(ip2))
		{
			CallCnt++;
			return (true);
		}
	}
	if (CallCnt == 3)
	{
		//if (TSVcreateClient("12.12.12.12"))
		if (TSVcreateClient(ip3))
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
	pthread_sigmask(SIG_UNBLOCK, &newMask, NULL);
	act.sa_handler = THRsigHandler;
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGCLD, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
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

	return (true);
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
	//// 작업 환경 초기화
	//initOK = TSVinitEnv();
	
	//while (true)
	//{
	//	ThrServer.MarkTime();
	//	printf("THRserver run");
	//	ThrServer.UpdateRunInfo();		// 실행 정보 갱신
	//	ThrServer.Pause(5);			// 5 msec
	//	ThrServer.Pause(500);			// 500 msec
	//}
	
	
	// 작업 환경 초기화
	initOK = TSVinitEnv();

	// Main loop
	while (initOK && !ThrServer.Terminate && !NeedTerminate())
	{
		ThrServer.MarkTime();

		// 접속 처리
		if (!TSVmanage())
			break;
		//printf("##[MTHR]THRserver run##\n");
		//Log.Debug("THRserver run");
		ThrServer.UpdateRunInfo();		// 실행 정보 갱신
		ThrServer.Pause(500);			// 5 msec
	}

	// 작업 환경 정리
	TSVclearEnv();	
}