//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "SharedMem.h"
#include "CLSmemory.h"
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
bool InitMemory(void);
bool InitMemory(void);
//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
CLSmemory	ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
SHARED_MEM	*ShmPtr = NULL;
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
	char buf[1024];

	printf("main start\n");
	printf("main end\n");

	if (InitMemory())
		printf("Create Memory\n");
	else
		printf("Create Memory Fail\n");

	
	printf("Terminate:%d\n", ShmPtr->Terminate);

	for (int i = 0; i < 10; i++)
	{
		printf("shrArr:%d\n", ShmPtr->shrArr[i]);
	}
	ShmMemory.Delete();
	return 0;
}

bool InitMemory(void)
{
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Create()) == (void *)-1)
		return (false);

	return (true);
}

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