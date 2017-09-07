//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSvimsIF.h"
//------------------------------------------------------------------------------
// Extern
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern CLSprocess *ShmPrc;
extern CLSsystem *ShmSys;
extern SHARED_MEM *ShmPtr;
//------------------------------------------------------------------------------
// CLSvimsIFH
//------------------------------------------------------------------------------
CLSvimsIFH::CLSvimsIFH(void)
{

}
//------------------------------------------------------------------------------
CLSvimsIFH::CLSvimsIFH(const char *name, int port, const char *ipAddr, TCP_MODE mode)
	: CLStcp(name, port, ipAddr, mode)
{

}
//------------------------------------------------------------------------------
CLSvimsIFH::CLSvimsIFH(const char *name, int port, int id, TCP_MODE mode)
	: CLStcp(name, port, id, mode)
{

}
//------------------------------------------------------------------------------
// ~CLSvimsIFH
//------------------------------------------------------------------------------
CLSvimsIFH::~CLSvimsIFH(void)
{

}
//------------------------------------------------------------------------------
// CheckTOMinfo
//------------------------------------------------------------------------------
bool CLSvimsIFH::CheckTOMinfo(void)
{
	BYTE code = m_message[VIMS_OPCODE];
	int sequence = GetNumber(m_message[VIMS_SEQ]);

	// 수신대기 상태확인
	if (!ptr->waiting)
		return (false);
	// 일련번호 확인
	if (ptr->sequence != sequence)
		return (false);
	// 대기중인 정보확인
	switch (ptr->code) // send message code
	{
	case VIMS_HEART_REQ:
		if (code != VIMS_HEART_RES)
			return (false);
		break;		// 송신에 대한 수신 확인
	default:	return (false);
	}
	Log.Debug("Receive the waiting message");
	ptr->waiting = false;
	return (true);
}
//------------------------------------------------------------------------------
// InitComState
//------------------------------------------------------------------------------
void CLSvimsIFH::InitComState(bool connected)
{
	m_lastClockSend = -1;
	m_txSequence = -1;
	m_accepted = false;
	memset(&m_tomInfo, 0, TOM_INFO_SIZE);
	memset(&m_txStat, 0, VIMS_TXSTAT_SIZE);
	SetRxState(VIMS_OPCODE);
	SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
}
//------------------------------------------------------------------------------
// ManageConnection
//------------------------------------------------------------------------------
CON_RESULT CLSvimsIFH::ManageConnection(void)
{
	bool tryConnect = false;
	CON_RESULT status;

	// 연결 상태 확인
	if (Connected)
		return (CON_OK);
	// 초기화 상태 확인
	if (!Initialized && !Open())
		return (CON_ERROR);
	/*
	// 연결 요구
	if ((status = Connect()) == CON_OK)
		InitComState(true);
	*/
	return (status);
}
//------------------------------------------------------------------------------
// ManageRX
//------------------------------------------------------------------------------
bool CLSvimsIFH::ManageRX(void)
{
	int count;
	char buffer[VIMS_TCPBUF_LEN];

	// Read
	if ((count = Read(buffer, VIMS_TCPBUF_LEN)) <= 0)
	{
		if (count < 0)
			Log.Write(m_id, 1, "%s Abnomal read [%d]", m_stamp, errno);
		return ((count < 0) ? false : true);
	}
	// Process packet
	RxHandler(buffer, count);
	return (true);
}
void CLSvimsIFH::RxHandler(char *buffer, int count)
{
	int remain, request;
	char *dPtr, *sPtr;	// srcPtr, destPtr;
	char stamp[SHORTBUF_LEN];

	Log.FLdump(1, "RAW", buffer, length, length);
	for (m_index = 0; m_index < length; )
	{
		dPtr = &m_message[m_tally];
		sPtr = buffer + m_index;
		switch (m_state)
		{
		case VIMS_CODE:
		case VIMS_SEQ1:
		case VIMS_SEQ2:
		case VIMS_DEVID1:
		case VIMS_DEVID2:
		case VIMS_DEVID3:
		case VIMS_DEVID4:
		case VIMS_LEN1:
			*dPtr = *sPtr;
			SetRxState((RX_STATE)(m_state + 1), 1);
			break;
		case VIMS_LEN2:
			*dPtr = *sPtr;
			m_length = GetNumber(&m_message[VIMS_LEN]);
			SetRxState(VIMS_DATA, 1);
		case VIMS_DATA:
			remain = length - m_index;
			request = m_length - m_tally;
			if (remain > 0 && request > 0)
			{
				request = min(remain, request);
				memcpy(dPtr, sPtr, request);
				SetRxState(VIMS_DATA, request);
			}
			// Um.. why?
			if (m_tally < m_length)
				break;
			
			MsgHandler();
		default:
			SetRxState(VIMS_CODE);	break;
		}
	}
}
//------------------------------------------------------------------------------
// MsgHandler
//------------------------------------------------------------------------------
void CLSvimsIFH::MsgHandler(void)
{
	int level; 
	bool waitChecked;
	BYTE code = m_message[VIMS_CODE];
	char stamp[SHORTBUF_LEN];

	// 수신메시지 확인
	m_rxSequence = GetNumber(m_message[VIMS_SEQ1]);
	sprintf(stamp, "%s RX", m_stamp);
	Log.FLdump(m_id, 1, stamp, m_message, m_tally, m_tally);

	waitChecked = CheckTOMinfo();

	// 수신메시지 처리
	switch (code)
	{	
	case VIMS_AUTHEN:
		// ACK or NACK
		break;
	case VIMS_CONFIG_REQ:
		// VIMS_CONFIG_RES
		break;
	case VIMS_VER_REQ:
		// VIMS_VER_RES
		break;
	case VIMS_RCPT_REQ:
		// VIMS_RCPT_RES
		break;
	case VIMS_VH_REQ:
		// VIMS_VH_RES
		break;
	case VIMS_SPEC_REQ:
		// VIMS_SPEC_RES
		break;
	case VIMS_TEST_START:
		// ACK
		break;
	case VIMS_SSR_RST:
		// ACK
		break;
	case VIMS_HL_RST:
		// ACK
		break;
	case VIMS_BS_RST:
		// ACk
		break;
	case VIMS_HEART_REQ:
		// VIMS_HEART_RES
		if (waitChecked)
			PrcHeartbeat();
		break;
	}
}
//------------------------------------------------------------------------------
// ManageTimeout
//------------------------------------------------------------------------------
bool CLSvimsIFH::ManageTimeout(void)
{
	TOM_INFO *ptr = &m_tomInfo;

	// 수신대기 상태 확인
	if (!ptr->waiting)	// waiting message가 없으면
		return (true);

	// 대기시간 경과 여부 확인
	if (!CheckElapsedTime(&ptr->txTime, ptr->timeout))
		return (true);

	// Timeout 이상 처리 여부 결정
	Log.Write((m_id, 2, "%s Timeout %d/%d [%02x]", m_stamp, ptr->count, VIMS_TIMEOUT_MAX, ptr->code);
	if (++ptr->count >= VIMS_TIMEOUT_MAX)
	{
		ptr->waiting = false;
		return (true);
	}

	// 정보 재전송
	if (!SendMessage())
		return (true);

	return (true);
}
//------------------------------------------------------------------------------
// ManageTX
//------------------------------------------------------------------------------
bool CLSvimsIFH::ManageTX(void)
{
	TOM_INFO *ptr = &m_tomInfo;

	// 수신대기 상태 확인
	if (ptr->waiting)
		return (true);

	// 접속 인증 확인

	// 로그인 확인

	// 설정정보 전송 확인

	// 버전정보 전송 확인

	// 접수정보 요청 확인

	return (true);
}
//------------------------------------------------------------------------------
// SetRxState
//------------------------------------------------------------------------------
void CLSvimsIFH::SetRxState(RX_STATE state, int delta)
{
	m_state = state;
	switch (state)
	{
	case VIMS_CODE:
		m_tally = 0;
		if (delta)
			m_index += delta;
		break;
	default:
		m_tally += delta;
		m_index += delta;
		break;
	}
}
//------------------------------------------------------------------------------
// SetTOMinfo
//------------------------------------------------------------------------------
void CLSvimsIFH::SetTOMinfo(int length, BYTE code, char *message, int timeout)
{
	TOM_INFO *ptr = &m_tomInfo;

	ptr->count = 0;
	ptr->length = length;
	ptr->code = code;
	ptr->sequence = m_txSequence;
	ptr->timeout = timeout;
	memcpy(ptr->message, message, length);
	gettimeofday(&ptr->txTime, NULL);
	ptr->waiting = true;		// 수신 받을 메세지가 있음.
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
bool CLSvimsIFH::Manage(void)
{
	time(&m_curClock);		// 현재 시각 갱신

							// Manage RX Timeout
	if (!ManageTimeout())
		return (true);

	// Manage RX
	if (!ManageRX())
	{
		Log.Write(m_id, 1, "%s RX fail", m_stamp);
		Close();
		return (false);
	}
	// Manage TX
	if (!ManageTX())
	{
		Log.Write(m_id, 1, "%s TX fail", m_stamp);
		Close();
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// PrcHeartbeat
//------------------------------------------------------------------------------
void CLSvimsIFH::PrcHeartbeat(void)
{
	
}