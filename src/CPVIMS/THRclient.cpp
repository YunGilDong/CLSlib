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
	// Terminate thread
	if (info->Thread != NULL)
		info->Thread->Stop();
}
//------------------------------------------------------------------------------
// THRclient
//------------------------------------------------------------------------------
void *THRclient(void *data)
{
	bool initOK;
	CLSequip *info;
	CLSthreadC *pThread = NULL;

	// 작업 환경 초기화
	info = (CLSequip *)data;
	pThread = info->Thread;
	initOK = TCLinitEnv(info);

	Log.Write("THRclient log address %d ", Log);
	// Main loop
	while (initOK && !pThread->Terminate && !NeedTerminate())
	{
		Log.Debug("THRclient run##");
		pThread->MarkTime();

		pThread->UpdateRunInfo();	// 실행 정보 갱신
		pThread->Pause(1000);		// 10 msec
	}
	TCLclearEnv(info);			// 작업 환경 정리
}
