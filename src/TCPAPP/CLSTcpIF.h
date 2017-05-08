//---------------------------------------------------------------------------
#ifndef CLSTcpIFH
#define CLSTcpIFH
//---------------------------------------------------------------------------
// Include
//---------------------------------------------------------------------------
#include "CLStcp.h"
#include "CLSlog.h"
//---------------------------------------------------------------------------
// Constant
#define	TCPBUF_LEN			1024
#define	TIMEOUT_MAX			3				// Max Timeout
//---------------------------------------------------------------------------
// Protocol
//---------------------------------------------------------------------------
// Header Length
//---------------------------------------------------------------------------
// Type definition
//---------------------------------------------------------------------------
// RX_STATE
//---------------------------------------------------------------------------
typedef enum { COMST_STX, COMST_DATA } RX_STATE;
//---------------------------------------------------------------------------
// TOM_INFO
//---------------------------------------------------------------------------
typedef struct
{
	int count;			// timeout count;
	int length;			// 송신 정보수
	int timeout;		// timeout 대기시간(msec)
	BYTE code;			// 송신 코드
	BYTE sequence;		// 송신 일련번호
	bool waiting;		// 수신 대기 여부
	char message[TCPBUF_LEN];	//송신 메시지
	struct timeval txTime;		// 송신 시간
} TOM_INFO;

#define TOM_INFO_SIZE	sizeof(TOM_INFO);
//---------------------------------------------------------------------------
// TX_STAT
//---------------------------------------------------------------------------
typedef struct
{
	bool requestConnAuthen;
	bool resConnAuthenAck;
	bool requestConfig;
	bool resConfig;
	bool requestVersion;
	bool resVersion;
	bool requestLogin;
	bool resLoginAck;
}BSHL_TXSTAT;

#define BSHL_TXSTAT_SIZE	sizeof(BSHL_TXSTAT);
//---------------------------------------------------------------------------
// Class
//---------------------------------------------------------------------------
class CLSTcpIF
{
private:
	int m_id;
	int m_index;
	int m_length;
	int m_tally;
	time_t m_curClock;
	char m_message[TCPBUF_LEN];
	char m_stamp[SHORTBUF_LEN];
	BYTE m_control;
	BYTE m_sfn;
	BYTE m_tfn;
	WORD m_destination;
	WORD m_rxSequence;
	WORD m_source;
	WORD m_txSequence;
	RX_STATE m_state;
	TOM_INFO m_tomInfo;
	CLStcp mTcp;
	CLSlog mLog;

	void CloseNetwork(char *message);
	void InitTxRxCode();	
	void InitComState(bool connected = false);
	CON_RESULT	ManageConnection(void);
	bool ManageRX(void);
	bool ManageTimeout(void);
	bool ManageTX(void);
	void RxHandler(char *buffer, int length);
	void MsgHandler(void);
	bool CheckTOMinfo(void);
	int  GetRxCodeAboutTx(BYTE txCode);
	bool ChkTxRxCode(BYTE txCode, BYTE rxCode);
	void SetRxState(RX_STATE state, int delta = 0);
	void SetTOMinfo(int length, BYTE code, char *message, int timeout = 300);

public:
	CLSTcpIF(void);
	CLSTcpIF(const char *name, int port, const char *ipAddr, TCP_MODE mode = TCP_SERVER);
	CLSTcpIF(const char *name, int port, int id, TCP_MODE mode = TCP_SERVER);
	~CLSTcpIF(void);

	bool Manage(void);
};
#endif
//---------------------------------------------------------------------------
// End of CLSTcpIF.h
//---------------------------------------------------------------------------