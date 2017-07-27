//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "SIGTEST.h"
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

CLSlog Log("SIGTEST", DIR_LOG); 
ULONG PRCID = 0;
//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	Log.Debug("Signal %d accepted\n", sig);
	printf("Signal %d accepted\n", sig);
	switch (sig)
	{
	case SIGTERM:	// 15
	case SIGKILL:	// 9
		printf("kill prc [%d]", PRCID);
		ClearEnv();
		break;
	case SIGINT:	//2		
		printf("SIGINT:%d\n", SIGINT);
		kill(PRCID, SIGQUIT);
		break;
	case SIGQUIT:
		printf("SIGQUIT:%d\n", SIGQUIT);
		kill(PRCID, SIGTERM);
		break;
	default:	break;
	}
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
	printf("InitEnv[1]");
	PRCID = getpid();
	Log.Write("Process start [%d]", getpid());

	InitSignal();
	printf("InitEnv[2]");

	ShmPrc = new CLSprocess("SIGTEST");
	printf("InitEnv[3]");

	ShmPrc->Register(getpid());
	printf("InitEnv[4]");

	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	ShmPrc->Deregister(getpid());	// Deregister process
	Log.Write("Process terminate [%d]", getpid());
	printf("Process terminate [%d]", getpid());
	exit(0);
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;
	printf("main start");
	initOK = InitEnv(argc, argv);
	
	// Main Loop
	while (initOK)
	{
		ShmPrc->MarkTime();		// 시작 시각 기록		
		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(1000);			// 100 msec
	}
	
	printf("main end");

	//ClearEnv();
	return (0);
}