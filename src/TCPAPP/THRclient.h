//---------------------------------------------------------------------------
#ifndef THRclientH
#define THRclientH
//---------------------------------------------------------------------------
// Include
//---------------------------------------------------------------------------
#include "CLSthread.h"
#include "CLSlog.h"
#include "CLSTcpIF.h"
//---------------------------------------------------------------------------
// Constant
//---------------------------------------------------------------------------
// Class
//---------------------------------------------------------------------------
class THRclient : public CLSthread
{
private:
	CLSTcpIF m_tcpClient;

	bool TCLmanage(void);

protected:
	virtual void Run(void);

public:
	THRclient(void);
	THRclient(CLSTcpIF *tcpClient);
	~THRclient(void);

	void start(void);
	void stop(void);
};
#endif
//---------------------------------------------------------------------------
// End of THRclient.h
//---------------------------------------------------------------------------