//------------------------------------------------------------------------------
#ifndef CLSprcthrcH
#define CLSprcthrcH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSthreadC.h"
#include "Database.h"
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
typedef struct
{
	char address[IPADDR_LEN];
} THRC_MNG;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSprcthrc : public CLSobject
{
private:
	void InitState(DB_PRCTHRC *pInfo);
public:
	int ID;
	//int Socket;
	bool Active;
	CLSthreadC *Thread;
	THRC_MNG Mng;
	CLSprcthrc(void);
	~CLSprcthrc(void);

	bool CheckState(void);
	void Delete(void);
	void Init(void);
	void Init(DB_PRCTHRC *pInfo);
};
//------------------------------------------------------------------------------
#endif // !CLSprcthrcH
//------------------------------------------------------------------------------
// End of CLSprcthrc.h
//------------------------------------------------------------------------------
