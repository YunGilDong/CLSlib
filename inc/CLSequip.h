//------------------------------------------------------------------------------
#ifndef CLSequipH
#define CLSequipH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLStcp.h"
#include "CLSthreadC.h"
#include "Database.h"
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// EQUIP_MNG
//------------------------------------------------------------------------------
typedef struct
{
	int id;	// Ispt Equip Id;
	BYTE type;	// Ispt equip Type
	char address[IPADDR_LEN];
} EQUIP_MNG;
//------------------------------------------------------------------------------
// EQUIP_OPR
//------------------------------------------------------------------------------
typedef struct
{
	bool stsCom;
} EQUIP_OPR;

#define EQUIP_OPR_SIZE		sizeof(EQUIP_OPR);
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSequip : public CLSobject
{
private:
	void InitState(DB_EQUIP *pInfo);
public:
	int ID;
	int Socket;
	bool Active;
	EQUIP_MNG Mng;
	EQUIP_OPR Opr;
	CLStcp *TcpIF;
	CLSthreadC *Thread;

	CLSequip(void);
	~CLSequip(void);

	bool CheckState(void);
	bool CheckState(int mode, bool state);
	void Delete(void);
	void Init(void);
	void Init(DB_EQUIP *pInfo);
};
//------------------------------------------------------------------------------
#endif // !CLSequipH
//------------------------------------------------------------------------------
// End of CLSequip.h
//------------------------------------------------------------------------------
