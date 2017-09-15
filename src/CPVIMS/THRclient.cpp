//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "THRclient.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern void THRsigHandler(int);
extern bool NeedTerminate(void);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void TCLclearEnv(CLSequip *);
//------------------------------------------------------------------------------
// TCLsigHandler
//------------------------------------------------------------------------------
void TCLsigHandler(CLSequip *ptr, int sig)
{
	Log.Write(1, "CL%d Signal %d accepted", ptr->ID, sig);
	switch (sig)
	{
	case SIGUSR1:	// 10
	case SIGSEGV:	// 11
		TCLclearEnv(ptr);
		break;
	case SIGPIPE:	// 13
	default:
		break;
	}
}
//------------------------------------------------------------------------------
// TCLinitNetwork
//------------------------------------------------------------------------------
bool TCLinitNetwork(CLSequip *info)
{
	char name[SHORTBUF_LEN];
	CLSthreadC *ptr = info->Thread;

	// Create TCP
	sprintf(name, "VIMSTCP%d", info->ID);
	if ((info->TcpIF = new CLSvimsIF(name, 0, info->Socket, TCP_CHILD)) == NULL)
	{
		Log.Write("CLient TCP create fail");
		return (false);
	}
	((CLSvimsIF *)info->TcpIF)->SetID(info->ID, info);
}
//------------------------------------------------------------------------------
// TCLinitSignal
//------------------------------------------------------------------------------
void TCLinitSignal(void)
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
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGCLD, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
}
//------------------------------------------------------------------------------
// TCLinitEnv
//------------------------------------------------------------------------------
bool TCLinitEnv(CLSequip *info)
{
	// Signal 처리기 초기화
	pthread_detach(pthread_self());
	TCLinitSignal();

	return (true);
}
//------------------------------------------------------------------------------
// TCLclearEnv
//------------------------------------------------------------------------------
void TCLclearEnv(CLSequip *info)
{
	Log.Write("Client Thread clear %d [%d]", info->ID, info->Thread->ID);

	// Close client Socket
	if (info->TcpIF != NULL)
	{
		Log.Write("Socket Close");
		info->TcpIF->Close();
	}
	// Terminate thread
	if (info->Thread != NULL)
	{
		Log.Write("thread stop");
		info->Thread->Stop();
	}
}
//------------------------------------------------------------------------------
// THRclient
//------------------------------------------------------------------------------
void *THRclient(void *data)
{
	bool initOK;
	int id=0;
	int cycle = 0;
	CLSequip *info;
	CLSthreadC *pThread = NULL;

	// 작업 환경 초기화
	info = (CLSequip *)data;
	pThread = info->Thread;
	initOK = TCLinitEnv(info);
	id = info->ID;

	pthread_t thrid = pthread_self();
	Log.Write("Client Thread ID[%d]", thrid);


	Log.Write("THRclient log address %d ", Log);
	// Main loop
	while (initOK && !pThread->Terminate && !NeedTerminate())
	{	
		pThread->MarkTime();
		if ((++cycle) % 500 == 0)
		{
			Log.Write("##CLTHR[%d] VIMS [%d]", id, cycle);
			Log.Debug("##CLTHR[%d] VIMS [%d]", id, cycle);
			if (cycle == 100000000) cycle = 0;
		}
		// Client 통신 관리
		//if (!info->TcpIF->Manage())
		//	break;

		pThread->UpdateRunInfo();	// 실행 정보 갱신
		pThread->Pause(5);		// 500 msec
	}
	// 작업 환경 정리
	TCLclearEnv(info);
}
