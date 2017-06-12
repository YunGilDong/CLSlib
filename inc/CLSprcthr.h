//------------------------------------------------------------------------------
#ifndef CLSprcthrH
#define CLSprcthrH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "Database.h"
#include "THRtest.h"
#include "CLSobject.h"
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// PRC_MNG
//------------------------------------------------------------------------------
typedef struct
{
	int val;
} PRC_MNG;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSprcthr : public CLSobject
{
private:

public:
	int ID;
	bool Active;
	PRC_MNG Mng;
	THRtest *Thread;

	CLSprcthr(void);
	//CLSprcthr(THRtest *ptr);
	~CLSprcthr(void);

	void Delete(void);
	void Init(void);
	void Init(DB_PRCTHR *pInfo);
	void Update(DB_PRCTHR *pInfo);

};


#endif // CLSprcthrH
