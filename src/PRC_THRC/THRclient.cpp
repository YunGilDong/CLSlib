//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#include "THRclient.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern SHARED_MEM *ShmPtr;
extern CLSthreadC	ThrClient;

extern  bool NeedTerminate(void);
extern void THRsigHandler(int);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void TCLclearEnv(void);
//------------------------------------------------------------------------------
// TCLsigHandler
//------------------------------------------------------------------------------
void TCLsigHandler(int sig)
{
	Log.Debug("THRC Signal %d accepted", sig);
	Log.Write("THRC Signal %d accepted", sig);
	switch (sig)
	{
	case SIGUSR1:	// 10
	case SIGSEGV:	// 11
		TCLclearEnv();
		break;
	case SIGPIPE:
		break;
	default:	break;
	}
}
//------------------------------------------------------------------------------
// TCLmanage
//------------------------------------------------------------------------------
bool TCLmanage(void)
{
	return (true);
}
//------------------------------------------------------------------------------
// TCLinitSignal
//------------------------------------------------------------------------------
void TCLinitSignal(void)
{
	sigset_t newMask;
	struct sigaction act;

	// Signal ó���� �ʱ�ȭ
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
bool TCLinitEnv(void)
{
	pthread_detach(pthread_self());

	TCLinitSignal();

	return (true);
}
//------------------------------------------------------------------------------
// TCLclearEnv
//------------------------------------------------------------------------------
void TCLclearEnv(void)
{
	ThrClient.Stop();		// Terminate thread
}
//------------------------------------------------------------------------------
// THRclient
//------------------------------------------------------------------------------
void *THRclient(void *data)
{
	bool initOK;

	initOK = TCLinitEnv();	// �۾� ȯ�� �ʱ�ȭ

	// Main loop
	while (initOK && !ThrClient.Terminate && !NeedTerminate())
	{
		ThrClient.MarkTime();

		// ���� ����
		if (!TCLmanage())
			break;
		Log.Debug("THRclient run");
		ThrClient.UpdateRunInfo();	// ���� ���� ����
		ThrClient.Pause(1000);		// 10 msec
	}
	TCLclearEnv();		// �۾� ȯ�� ����
}