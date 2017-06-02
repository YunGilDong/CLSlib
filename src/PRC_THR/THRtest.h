#ifndef THRtestH
#define THRtestH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "General.h"
#include "CLSthread.h"
#include "CLSlog.h"
#include <signal.h>
#include <unistd.h>
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class THRtest : public CLSthread
{
private:

	
protected:
	virtual void Run(void);
public:
	int m_num;

	THRtest(void);
	THRtest(const char *name);
	THRtest(const char *name, int interval = DEF_CHECK_INTERVAL);
	~THRtest(void);

	bool manage(void);
	bool start();
	void stop();

	void test();
};

#endif // THRtestH