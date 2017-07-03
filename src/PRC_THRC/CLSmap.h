//------------------------------------------------------------------------------
#ifndef CLSmapH
#define CLSmapH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "Database.h"
#include "CLSprcthrc.h"
//------------------------------------------------------------------------------
#include <map>
#include <string>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// MAP_CLIENT
//------------------------------------------------------------------------------
typedef map <string, CLSprcthrc *> MAP_CLIENT;
typedef MAP_CLIENT::iterator MPCL_IT;
//------------------------------------------------------------------------------
// MAP_DB
//------------------------------------------------------------------------------
typedef map <string, CLSprcthrc *> MAP_DB;
typedef MAP_DB::iterator MPDB_IT;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSmap : public CLSobject
{
private:
	MAP_DB m_dbase;
public:
	MAP_CLIENT Client;

	CLSmap(void);
	CLSmap(const char *name);
	~CLSmap(void);

	bool Add(char *id, CLSprcthrc *ptr);
	bool AddDB(char *address, CLSprcthrc *ptr);
	void Erase(char *id);
	CLSprcthrc *Get(char *id);
	CLSprcthrc *Get(pthread_t id);
	CLSprcthrc *GetDB(char *address);
};
//------------------------------------------------------------------------------
#endif // CLSmapH
//------------------------------------------------------------------------------
// End of CLSmap.h
//------------------------------------------------------------------------------

