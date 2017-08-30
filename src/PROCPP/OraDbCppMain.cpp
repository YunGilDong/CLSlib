//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "OraDbCppMain.h"
//------------------------------------------------------------------------------
// initEnv
//------------------------------------------------------------------------------
bool initEnv()
{
	// DB connect
	if (DBaseConnect_cpp(DB_USER, DB_PWD, DB_ALIAS) < 0)
	{
		printf("Database connect fail\n");
		return (false);
	}
	printf("Database connect success\n");

	//YGD_DB info[3];
	//DBSelectTest_cpp(info, 3);
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv()
{
	// DB disconnect	
	if (DBaseDisconnect_cpp() < 0)
		printf("Database disconnect fail\n");
	else
		printf("Database disconnect success\n");
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;
	char ch;

	// InitEnv
	initOK = initEnv();
	initOK = true;
	while (initOK)
	{
		printf(">>");
		scanf("%c", &ch);
		if (ch == 'q' || ch == 'Q')
			break;
	}

	// clear work env
	ClearEnv();
	return (0);
}