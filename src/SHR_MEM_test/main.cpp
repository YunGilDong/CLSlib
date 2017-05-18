//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "SharedMem.h"
#include "CLSmemory.h"
#include "CLSprocess.h"
#include <sys/types.h>
#include <unistd.h>
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
bool InitMemory(void);
//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
CLSmemory	ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
SHARED_MEM	*ShmPtr = NULL;
CLSprocess *ShmPrc = NULL;


//------------------------------------------------------------------------------
// InitEnv
//------------------------------------------------------------------------------
bool InitEnv(int argc, char **argv)
{	
	printf("Process start [%d]", getpid());
	ShmPrc->Register(getpid());
	return (true);
}
//------------------------------------------------------------------------------
// InitMemory
//------------------------------------------------------------------------------
bool InitMemory(void)
{
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Create()) == (void *)-1)
		return (false);

	// test
	for (int i = 0; i<10; i++)
		ShmPtr->shrArr[i] = i + 10;
	ShmPtr->Terminate = true;
	ShmPrc = &ShmPtr->process[PRC_SHMTEST];

	return (true);
}
//------------------------------------------------------------------------------
// InitMemory2
//------------------------------------------------------------------------------
bool InitMemory2(void)
{
	int m_id = 0;
	// Shared Memory를 생성한다.
	if ((m_id = shmget(0x100, SHARED_MEM_SIZE, 0x1B6 | IPC_CREAT)) < 0)
	{
		printf("fail");
	}
	printf("m_id:%d\n", m_id);

	void *m_address;
	m_address = shmat(m_id, 0, 0x1B6);
	printf("address:%d\n", m_address);


	// Shared memory 생성 여부 확인
	if ((m_id = shmget(0x100, SHARED_MEM_SIZE, 0x1B6)) < 0)
	{
		printf("fail");
		return ((void *)-1);
	}
	else
		printf("mid : %d\n", m_id);


	return (true);
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	char buf[1024];
	
	printf("main start\n");
	

	//InitEnv(argc, argv);

	if (InitMemory())
		printf("Create Memory\n");
	else
		printf("Create Memory Fail\n");

	scanf("%s", buf);

	ShmMemory.Delete();
	printf("main end\n");
	return 0;
}