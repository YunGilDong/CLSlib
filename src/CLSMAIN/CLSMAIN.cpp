//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSMAIN.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------
CLSlog Log("CLSMAIN", DIR_LOG);
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
bool Terminate = false;
CLSprocess *ShmPrc = NULL;
CLSmemory   ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
PRC_DESC PrcDesc[MAX_PROCESS] = { 
				{"CLSMAIN",""},
				{"SHM_TEST",SHM_PRC1 }
			};

//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	switch (sig)
	{
	case SIGTERM:	//15
	case SIGKILL:	//9
		Log.Write("Signal %d accepted", sig);
		Terminate = true;
		break;
	case SIGCHID:	//17
	case SIGUSR1:   //10
	default:
		Log.Write("Signal %d accepted", sig);
		break;
	}
}
//------------------------------------------------------------------------------
// NeedTerminate
//------------------------------------------------------------------------------
bool NeedTerminate(void)
{
	if (Terminate)
		return (true);
	if (ShmSys->Terminate)
		return (true);

	return (false);
}
//------------------------------------------------------------------------------
// ManageProcess
//------------------------------------------------------------------------------
void ManageProcess(void)
{
	RUN_STATE state;
	CLSprocess *ptr = ShmPtr->process;

	// ���μ��� � ���� Ȯ��
	for (int idx = PRC_CLSMAIN; idx < MAX_PROCESS; idx++, ptr++)
	{
		// ���μ��� Ȱ�� ���� �� ���� ���� Ȯ��
		if (!ptr->Active)
			continue;
		if (ptr->IsRunning(&state))
			continue;

		Log.Write("%s Run State : %d", ptr->Name, state);
		switch (state)
		{
		case RST_ABNOMAL: ptr->Kill();
		case RST_UNEXIST: ptr->Start();	break;
		default: break;
		}
	}
}
//------------------------------------------------------------------------------
// ManageDebug
//------------------------------------------------------------------------------
void ManageDebug(void)
{
	if (!ShmPrc->ChangeDebug)
		return;

	ShmPrc->SetDebug(ShmPrc->ReqLevl, ShmPrc->ReqTarget);
	Log.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	ShmPrc->ChangeDebug = false;
}
//------------------------------------------------------------------------------
// InitProcess
//------------------------------------------------------------------------------
bool InitProcess(void)
{
	PRC_DESC *dPtr = PrcDesc;
	CLSprocess *ptr = ShmPtr->process;

	for (int idx = PRC_CLSMAIN; idx < MAX_PROCESS; idx++, ptr++, dPtr++)
	{
		if (!strlen(dPtr->name))
			break;

		if (idx == PRC_CLSMAIN)
			ptr->Init(dPtr->name);
		else if (!ptr->Start(dPtr))
			return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// TerminateProcess
//------------------------------------------------------------------------------
void TerminateProcess(void)
{
	CLSprocess *ptr;

	// �������� �����޸� ������ �ƴ� ��� �����Ѵ�.
	if (ShmPtr == NULL)
		return;

	for (int idx = PRC_CLSMAIN; idx < MAX_PROCESS;)
	{
		if (idx == PRC_CLSMAIN)
		{
			idx++;
			continue;
		}
		// ���μ����� Ȱ�� ���� �� ���� ���� Ȯ��
		ptr = &ShmPtr->process[idx];
		if (!ptr->Active || !ptr->IsExist())
		{
			idx++;
			continue;
		}
		// ���μ����� �����Ų��.
		ptr->Kill();
		usleep(50000); // 50 msec
	}
}
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEF_CLSMAIN_LEVEL);
	ManageDebug();
}
//------------------------------------------------------------------------------
// InitMemory
//------------------------------------------------------------------------------
bool InitMemory(void)
{
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Create()) == (void *)-1)
		return (false);

	ShmSys = &ShmPtr->system;
	ShmPrc = &ShmPtr->process[PRC_CLSMAIN];
	return (true);
}
//------------------------------------------------------------------------------
// InitOption
//------------------------------------------------------------------------------
void InitOption(int argc, char **argv)
{
}
//------------------------------------------------------------------------------
// InitSignal
//------------------------------------------------------------------------------
void InitSignal(void)
{
	signal(SIGABRT, SigHandler);


}