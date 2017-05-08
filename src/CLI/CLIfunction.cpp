//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "CLI.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern bool	Terminate;
extern SHARED_MEM	*ShmPtr;
extern CLSsystem *ShmSys;
extern CLI_INFO CliInfo;
extern CLScommand	CmdTable[];

//------------------------------------------------------------------------------
extern bool CheckEOT(int);
extern void PrnLine(char *, char, bool);
extern void PrnError(const char *, ...);
extern CLSprocess *GetProcess(void);
//------------------------------------------------------------------------------
// CmdHelp
//------------------------------------------------------------------------------
void CmdHelp(void)
{
	int idx;
	CLScommand *ptr = CmdTable;

	// ��ü ��ɾ� ǥ�� ���� Ȯ��
	if (CheckEOT(EOT_DONTCARE))
	{
		PrnLine("[����]", '=', true);
		printf("  %-16s ����\n", "���");
		PrnLine("", '-', true);
		for (idx = 0; idx < MAX_CLICMD; idx++, ptr++)
			ptr->PrintInfoShort();
		PrnLine("[����]", '=', false);
		return;
	}
	// Display request command usage
	for (idx = 0; idx < MAX_CLICMD; idx++, ptr++)
	{
		if (!ptr->Check(CliInfo.token, false))
			continue;
		if (!CheckEOT(EOT_VALID))
			return;

		PrnLine("[����]", '=', true);
		printf("  ���:\t%s\r\n", ptr->Command);
		PrnLine("", '-', false);
		ptr->PrintInfo();
		PrnLine("[����]", '=', false);
		return;
	}
	PrnError("%%%%ERR_'%s' ��(��) �����ǵ� ��ɾ� �Դϴ�.\n", CliInfo.token);
}
//------------------------------------------------------------------------------
// CmdKill
//------------------------------------------------------------------------------
void CmdKill(void)
{
	CLSprocess *ptr = ShmPtr->process;

	if ((ptr = GetProcess()) == NULL)
		return;
	if (!CheckEOT(EOT_VALID))
		return;

	ptr->Kill();
}
//------------------------------------------------------------------------------
// CmdExit
//------------------------------------------------------------------------------
void CmdExit(void)
{
	if (!CheckEOT(EOT_VALID))
		return;

	Terminate = true;
}