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
//extern void *THRclient(void *);
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
// CLStcp TcpServer("EQUIPSVTCP","TCP_SERVER_PORT, "")'

void TCLsigHandler(CLSequip *, int)
{

}
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

}
//------------------------------------------------------------------------------
// TSVmanage
//------------------------------------------------------------------------------
bool TSVmanage(void)
{

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

	// Signal ó���� �ʱ�ȭ
	sigemptyset(&newMask);
	sigaddset(&newMask, SIGSEGV);
	sigaddset(&newMask, SIGHUP);
	sigaddset(&newMask, SIGCLD);
	sigaddset(&newMask, SIGCHLD);
	sigaddset(&newMask, SIGPIPE);
	sigaddset(&newMask, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &newMask, NULL);	// thread signal �ʱ�ȭ, ������ �ʱ�ȭ�� �ñ׳θ� ���μ������� �޾Ƶ���
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
	// Signal ó���� �ʱ�ȭ
	pthread_detach(pthread_self());
	TSVinitSignal();

	// Network�ʱ�ȭ
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
	initOK = TSVinitEnv();	// �۾� ȯ�� �ʱ�ȭ

							// Main loop
	while (initOK && !ThrServer.Terminate && !NeedTerminate())
	{
		Log.Debug("THRserver run##"); 
		ThrServer.MarkTime();

		ThrServer.UpdateRunInfo();	// ���� ���� ����
		ThrServer.Pause(1000);		// 10 msec
	}
	//TCLclearEnv();		// �۾� ȯ�� ����
}