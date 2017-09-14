//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#include "THRclient.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;

extern void THRsigHandler(int);
extern bool NeedTerminate(void);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void TCLclearEnv(CLSprcmthr *);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// TCLsigHandler
//------------------------------------------------------------------------------
void TCLsigHandler(CLSprcmthr *ptr, int sig)
{
	Log.Write("Client signal %d accepted", sig);
	switch (sig)
	{
	case SIGPIPE:
	case SIGUSR1:
	case SIGSEGV: TCLclearEnv(ptr);	break;
	default:	break;
	}
}
//------------------------------------------------------------------------------
// TCLinitNetwork
//------------------------------------------------------------------------------
bool TCLinitNetwork(CLSprcmthr *info)
{

}
//------------------------------------------------------------------------------
// TCLinitSignal
//------------------------------------------------------------------------------
void TCLinitSignal(void)
{
	sigset_t newMask;
	struct sigaction act;

	// Signal ó���� �ʱ�ȭ
	sigemptyset(&newMask);		// signal set clear
	sigaddset(&newMask, SIGSEGV);
	sigaddset(&newMask, SIGHUP);
	sigaddset(&newMask, SIGCLD);
	sigaddset(&newMask, SIGPIPE);
	sigaddset(&newMask, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &newMask, NULL);
	act.sa_handler = THRsigHandler;
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGCLD, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
}
//------------------------------------------------------------------------------
// TCLinitEnv
//------------------------------------------------------------------------------
bool TCLinitEnv(CLSprcmthr *pMTHR)
{
	pthread_detach(pthread_self());
	TCLinitSignal();		// Signal ó���� �ʱ�ȭ

	return (true);
}
//------------------------------------------------------------------------------
// TCLSclearEnv
//------------------------------------------------------------------------------
void TCLclearEnv(CLSprcmthr *pMTHR)
{
	// Terminate thread
	if (pMTHR->Thread != NULL)
		pMTHR->Thread->Stop();
}
//------------------------------------------------------------------------------
// THRclient
//------------------------------------------------------------------------------
void *THRclient(void *data)
{
	bool initOK=true;
	CLSprcmthr *info;
	CLSthreadC *pThread = NULL;

	// �۾� ȯ�� �ʱ�ȭ
	info = (CLSprcmthr *)data;
	pThread = info->Thread;
	initOK = TCLinitEnv(info);

	while (initOK)
	{
		//Log.Debug("#THRclient RUN#");
		pThread->MarkTime();

		pThread->UpdateRunInfo();	// ���� ���� ����
		pThread->Pause(500);	// 500 msec
	}
	// �۾� ȯ�� ����
	TCLclearEnv(info);
}