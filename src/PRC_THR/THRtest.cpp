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
	printf("Thread Run \n");
	while (true)
	{
		//Log.Write("%d ", m_num++);
		m_num++;
		UpdateRunInfo();	// 실행 정보 갱신
		Pause(1000);			// 1000 msec
	}
}