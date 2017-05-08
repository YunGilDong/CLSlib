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

	// 전체 명령어 표시 여부 확인
	if (CheckEOT(EOT_DONTCARE))
	{
		PrnLine("[도움말]", '=', true);
		printf("  %-16s 설명\n", "명령");
		PrnLine("", '-', true);
		for (idx = 0; idx < MAX_CLICMD; idx++, ptr++)
			ptr->PrintInfoShort();
		PrnLine("[도움말]", '=', false);
		return;
	}
	// Display request command usage
	for (idx = 0; idx < MAX_CLICMD; idx++, ptr++)
	{
		if (!ptr->Check(CliInfo.token, false))
			continue;
		if (!CheckEOT(EOT_VALID))
			return;

		PrnLine("[도움말]", '=', true);
		printf("  명령:\t%s\r\n", ptr->Command);
		PrnLine("", '-', false);
		ptr->PrintInfo();
		PrnLine("[도움말]", '=', false);
		return;
	}
	PrnError("%%%%ERR_'%s' 은(는) 미정의된 명령어 입니다.\n", CliInfo.token);
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