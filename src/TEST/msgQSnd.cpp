//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "msgQ.h"
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
#define Q_KEY	0x100
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
CLSqueue MsgQue;
MSG_DESC MsgDesc[1] = { {Q_KEY, "TEST_Q"} };
//------------------------------------------------------------------------------
// InitQuere
//------------------------------------------------------------------------------
bool InitQueue(void)
{
	MSG_DESC *dPtr = MsgDesc;
	CLSqueue *ptr = &MsgQue;

	for (int idx = 0; idx < 1; idx++, dPtr++, ptr++)
	{
		printf("[1]\n");
		if (!ptr->Create(dPtr))
			return (false);
	}
	return (true); 
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	printf("MSG QUEUE TEST\n");
	int num = 0;
	char buffer[256];
	bool initOK;
	initOK = InitQueue();
	if (!initOK)
	{
		printf("Queue create fail\n");
		exit(0);
	}
	
	while (initOK)
	{		
		memset(buffer, 0x00, 256);
		MsgQue.SetNumber(buffer, num, 4);
		num++;
		if (!(MsgQue.Send(buffer, 4)))
		{
			printf("Queue Send fail\n");
			exit(0);
		}
		printf("Queue Send\n");
		sleep(2);	// 2 second
	}


	return (0);
}