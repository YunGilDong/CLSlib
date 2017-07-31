//------------------------------------------------------------------------------
#ifndef	SharedMemH
#define	SharedMemH
//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "Define.h"
#include "CLSsystem.h"
#include "CLSprocess.h"
#include "CLSprcthrc.h"
#include "CLSprcmthr.h"
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// Shared Memory Key
//------------------------------------------------------------------------------
#define	YGD_SHM_KEY		0x100
//------------------------------------------------------------------------------
// Process Index
//------------------------------------------------------------------------------
#define PRC_CLSMAIN			0
#define PRC_TEST01			PRC_CLSMAIN+1
#define PRC_THR				PRC_TEST01+1
#define PRC_THRC			PRC_THR+1
#define PRC_MTHR			PRC_THRC+1
#define MAX_PROCESS			PRC_MTHR+1
//------------------------------------------------------------------------------
// SHARED_MEM
//------------------------------------------------------------------------------
typedef struct
{
	CLSsystem	system;
	CLSprocess	process[MAX_PROCESS];
	CLSprcthrc	prcthrc[MAX_PRCTHRC];
	CLSprcmthr	prcmthr[MAX_PRCMTHR];

	int shrArr[10];
	bool Terminate;
} SHARED_MEM;

#define SHARED_MEM_SIZE		sizeof(SHARED_MEM)

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
// End of SharedMem.h
//------------------------------------------------------------------------------