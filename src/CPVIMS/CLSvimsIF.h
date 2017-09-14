//------------------------------------------------------------------------------
#ifndef CLSvimsIFH
#define CLSvimsIFH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "SharedMem.h"
#include "General.h"
#include "CLStcp.h"
#include "CLSequip.h"

//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
#define	VIMS_TCPBUF_LEN		1024
#define	VIMS_SHORTBUF_LEN	256
#define VIMS_HEADER_LEN		9
#define VIMS_TIMEOUT_MAX	3
#define VIMS_CHECK_INTERVAL	10000	// 10 sec
//------------------------------------------------------------------------------
// Protocol
//------------------------------------------------------------------------------
#define VIMS_PORT	5010
//------------------------------------------------------------------------------
#define VIMS_OPCODE		0
#define VIMS_SEQ		VIMS_OPCODE+1
#define VIMS_SEQ1		VIMS_SEQ+1
#define VIMS_SEQ2		VIMS_SEQ1+1
#define VIMS_DEV_ID		VIMS_SEQ2+1
#define VIMS_DEV_ID1	VIMS_DEV_ID
#define VIMS_DEV_ID2	VIMS_DEV_ID1+1
#define VIMS_DEV_ID3	VIMS_DEV_ID2+1
#define VIMS_DEV_ID4	VIMS_DEV_ID3+1
#define VIMS_LEN		VIMS_DEV_ID4+1
#define VIMS_LEN1		VIMS_LEN
#define VIMS_LEN2		VIMS_LEN1+1
#define VIMS_DATA		VIMS_LEN2+1
//------------------------------------------------------------------------------
#define VIMS_AUTHEN		0x10
#define VIMS_LOGIN		0x11
#define VIMS_LOGOUT		0x12
//------------------------------------------------------------------------------
#define VIMS_CONFIG_REQ	0x13
#define VIMS_CONFIG_RES	0x23
//------------------------------------------------------------------------------
#define VIMS_VER_REQ	0x14
#define VIMS_VER_RES	0x24
//------------------------------------------------------------------------------
#define VIMS_RCPT_REQ	0x31
#define	VIMS_RCPT_RES	0x41
//------------------------------------------------------------------------------
#define	VIMS_VH_REQ		0x32
#define VIMS_VH_RES		0x42
//------------------------------------------------------------------------------
#define VIMS_SPEC_REQ	0x33
#define VIMS_SPEC_RES	0x43
//------------------------------------------------------------------------------
#define VIMS_TEST_START 0x50
#define VIMS_SSR_RST	0x51
#define VIMS_HL_RST		0x52
#define VIMS_BS_RST		0x53
//------------------------------------------------------------------------------
#define VIMS_ACK		0xA0
#define VIMS_NACK		0xA1
//------------------------------------------------------------------------------
#define VIMS_HEART_REQ	0x70
#define VIMS_HEART_RES	0x80
//------------------------------------------------------------------------------
// OPOCDE Array Index (수신받은 메시지를 송신할때 사용)
//------------------------------------------------------------------------------
#define OP_AUTHEN		0
#define OP_LOGIN		1
#define OP_LOGOUT		OP_LOGIN+1
#define MAX_OP			OP_LOGOUT+1
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// RX_STATE
//------------------------------------------------------------------------------
typedef enum { CODE, SEQ1, SEQ2, DEVID1, DEVID2, DEVID3, DEVID4, LEN1, LEN2, DATA } RX_STATE;
//------------------------------------------------------------------------------
// TOM_INFO (송신 메시지에 대해 수신을 확인할 때)
//------------------------------------------------------------------------------
typedef struct
{
	int count;		// Timeout count
	int sequence;	// 송신 일련번호
	int length;		// 송신 정보수
	int timeout;	// Timeout 대기시간(msec)
	BYTE code;		// 수신대기 코드
	bool waiting;	// 수신 대기 여부
	char message[VIMS_TCPBUF_LEN];	// 송신 메시지
	struct timeval txTime;	// 송신 시각
} TOM_INFO;

#define TOM_INFO_SIZE		sizeof(TOM_INFO)
//------------------------------------------------------------------------------
// TOM_RCV_INFO (수신받은 메시지에 대해 송신을 확인 할 때)
//------------------------------------------------------------------------------
typedef struct
{
	int sequence;	// 수신 일련번호
	int length;		// 수신 정보수
	BYTE code;		// 수신받은 코드
	BYTE nackCode;	// NACK 코드
	bool waiting;	// 송신 대기 여부
	char message[VIMS_TCPBUF_LEN]; // 수신 메시지
	struct timeval rxTime;	// 수신 시각
}RCV_TOM_INFO;

#define RCV_TOM_INFO_SIZE	sizeof(RCV_TOM_INFO)
//------------------------------------------------------------------------------
// VIMS_TXSTAT
//------------------------------------------------------------------------------
typedef struct
{	
	bool resConfig;
	bool resVer;
	bool resRcpt;
	bool resVh;
	bool resSpec;
	bool resHeart;
	bool sendAck;
} VIMS_TXSTAT;

#define VIMS_TXSTAT_SIZE		sizeof(VIMS_TXSTAT)
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSvimsIF : public CLStcp
{
private:
	int m_id;	// 장치 ID
	int m_index;
	int m_lastClockSend;
	int m_length;
	int m_opID;
	int m_tally;
	bool m_accepted;
	bool m_active;
	WORD m_rxSequence;
	WORD m_txSequence;
	time_t m_curClock;
	time_t m_rxClock;
	char m_stamp[SHORTBUF_LEN];
	char m_message[VIMS_TCPBUF_LEN];
	CLSequip *m_pEquip;
	RX_STATE m_state;
	TOM_INFO m_tomInfo;
	RCV_TOM_INFO m_rTomInfo[MAX_OP];
	VIMS_TXSTAT m_txStat;
	struct tm *m_curTod;
	struct timeval m_statusTimer;

	bool CheckTOMinfo(void);
	void InitComState(bool connected = false);
	CON_RESULT ManageConnection(void);
	bool ManageRX(void);
	void RxHandler(char *buffer, int count);
	void MsgHandler(void);
	bool ManageTimeout(void);
	bool ManageTX(void);	
	void SetRxState(RX_STATE state, int delta = 0);
	void SetTOMinfo(int length, BYTE code, char *message, int timeout = 300);
	void SetRcvTOMinfo(int length, BYTE code, char *message);

	void PrcHeartbeat(void);
	void PrcAuthen(void);
		
	bool SendConfig(void);
	bool SendVerInfo(void);
	bool SendRcptInfo(void);
	bool SendVhInfo(void);
	bool SendSpecInfo(void);
	bool SendHeartbeat(void);
	bool SendAck(BYTE code, BYTE nackCode = 0);
	bool SendNAck(BYTE code);

	bool SendMessage(void);
	bool SendMessage(BYTE code, int length = 0, char *info = NULL);

	

public:
	CLSvimsIF(void);
	CLSvimsIF(const char *name, int port, const char *ipAddr, TCP_MODE mode= TCP_SERVER);
	CLSvimsIF(const char *name, int port, int id, TCP_MODE mode = TCP_SERVER);
	~CLSvimsIF(void);

	bool Manage(void);
	void SetID(int id, CLSequip *pEquip);
};
//------------------------------------------------------------------------------
#endif // !CLSvimsIFH
//------------------------------------------------------------------------------
// End of CLSvimsIF.h
//------------------------------------------------------------------------------