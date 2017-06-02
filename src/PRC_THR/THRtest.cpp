//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "THRtest.h"
//------------------------------------------------------------------------------
// Extern
//------------------------------------------------------------------------------
extern CLSlog Log;
extern THRtest ThrObj;
//------------------------------------------------------------------------------
// THRtest
//------------------------------------------------------------------------------
THRtest::THRtest(void)
{
	m_num = 10;
}
THRtest::THRtest(const char *name) : CLSthread(name, DEF_CHECK_INTERVAL)
{
	m_num = 10;
}
THRtest::THRtest(const char *name, int interval) : CLSthread(name, interval)
{
	m_num = 10;
}
THRtest::~THRtest(void)
{
}
bool THRtest::manage(void)
{
	Manage();
}
bool THRtest::start()
{
	return (Start());
}
void THRtest::stop()
{
	Stop();
}
void THRtest::test()
{

}
void THRtest::Run(void)
{
	pritnf("Thread Run \n");
	while (true)
	{
		printf("%d ", m_num++);
		usleep(100000);	// 1000msec
	}
}