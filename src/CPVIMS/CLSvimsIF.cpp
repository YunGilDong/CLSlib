//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSvimsIF.h"
#include "CLSprocess.h"
#include "CLSlog.h"
#include "CLSmap.h"
//------------------------------------------------------------------------------
// Extern
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern CLSprocess *ShmPrc;
extern CLSsystem *ShmSys;
extern SHARED_MEM *ShmPtr;
//------------------------------------------------------------------------------
// CLSvimsIF
//------------------------------------------------------------------------------
CLSvimsIF::CLSvimsIF(void)
{

}
//------------------------------------------------------------------------------
CLSvimsIF::CLSvimsIF(const char *name, int port, const char *ipAddr, TCP_MODE mode)
	: CLStcp(name, port, ipAddr, mode)
{

}
//------------------------------------------------------------------------------
CLSvimsIF::CLSvimsIF(const char *name, int port, int id, TCP_MODE mode)
	: CLStcp(name, port, id, mode)
{

}
//------------------------------------------------------------------------------
// ~CLSvimsIF
//------------------------------------------------------------------------------
CLSvimsIF::~CLSvimsIF(void)
{

}
//------------------------------------------------------------------------------
// CheckTOMinfo
//------------------------------------------------------------------------------
bool CLSvimsIF::CheckTOMinfo(void)
{
	BYTE code = m_message[VIMS_OPCODE];
	TOM_INFO *ptr = &m_tomInfo;
	int sequence = GetNumber(&m_message[VIMS_SEQ]);

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
void CLSvimsIF::InitComState(bool connected)
{
	m_lastClockSend = -1;
	m_txSequence = -1;
	m_accepted = false;
	memset(&m_tomInfo, 0, TOM_INFO_SIZE);
	memset(&m_txStat, 0, VIMS_TXSTAT_SIZE);
	SetRxState(CODE);
	SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
}
//------------------------------------------------------------------------------
// ManageConnection
//------------------------------------------------------------------------------
CON_RESULT CLSvimsIF::ManageConnection(void)
{
	bool tryConnect = false;
	CON_RESULT status;
	Log.Write("Manage CON");
	// 연결 상태 확인
	if (Connected)
	{
		Log.Write("TCP CON");
		return (CON_OK);
	}
	// 초기화 상태 확인
	if (!Initialized && !Open())
	{
		Log.Write("TCP CON ERR");
		return (CON_ERROR);
	}
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
bool CLSvimsIF::ManageRX(void)
{
	int count;
	char buffer[VIMS_TCPBUF_LEN];

	// Read
	if ((count = Read(buffer, VIMS_TCPBUF_LEN)) <= 0)
	{
		if (count < 0)
			Log.Write(1, "%s Abnomal read[%d] [%d] [%d]", m_stamp, count, errno, m_id);
		return ((count < 0) ? false : true);
	}
	// Process packet
	RxHandler(buffer, count);
	return (true);
}
void CLSvimsIF::RxHandler(char *buffer, int length)
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
		case CODE:
		case SEQ1:
		case SEQ2:
		case DEVID1:
		case DEVID2:
		case DEVID3:
		case DEVID4:
		case LEN1:
			*dPtr = *sPtr;
			SetRxState((RX_STATE)(m_state + 1), 1);
			break;
		case LEN2:
			*dPtr = *sPtr;
			m_length = GetNumber(&m_message[VIMS_LEN]);
			SetRxState(DATA, 1);
		case DATA:
			remain = length - m_index;
			request = m_length - m_tally;
			if (remain > 0 && request > 0)
			{
				request = min(remain, request);
				memcpy(dPtr, sPtr, request);
				SetRxState(DATA, request);
			}
			// Um.. why?
			if (m_tally < m_length)
				break;
			
			MsgHandler();
		default:
			SetRxState(CODE);	break;
		}
	}
}
//------------------------------------------------------------------------------
// MsgHandler
//------------------------------------------------------------------------------
void CLSvimsIF::MsgHandler(void)
{
	int level; 
	bool waitChecked;
	BYTE code = m_message[VIMS_OPCODE];
	char stamp[SHORTBUF_LEN];

	// 수신메시지 확인
	m_rxSequence = GetNumber(&m_message[VIMS_SEQ1]);
	sprintf(stamp, "%s RX", m_stamp);
	Log.FLdump(m_id, 1, stamp, m_message, m_tally, m_tally);

	waitChecked = CheckTOMinfo();

	// 수신메시지 처리
	switch (code)
	{	
	case VIMS_AUTHEN:
	case VIMS_LOGIN:
	case VIMS_LOGOUT:
		// ACK or NACK
		SetRcvTOMinfo(m_length, code, m_message);
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
bool CLSvimsIF::ManageTimeout(void)
{
	TOM_INFO *ptr = &m_tomInfo;

	// 수신대기 상태 확인
	if (!ptr->waiting)	// waiting message가 없으면
		return (true);

	// 대기시간 경과 여부 확인
	if (!CheckElapsedTime(&ptr->txTime, ptr->timeout))	// 대기시간이 설정한 타임아웃 시간 이내면
		return (true);

	// Timeout 이상 처리 여부 결정
	Log.Write(m_id, 2, "%s Timeout %d/%d [%02x]", m_stamp, ptr->count, VIMS_TIMEOUT_MAX, ptr->code);
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
bool CLSvimsIF::ManageTX(void)
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
void CLSvimsIF::SetRxState(RX_STATE state, int delta)
{
	m_state = state;
	switch (state)
	{
	case CODE:
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
void CLSvimsIF::SetTOMinfo(int length, BYTE code, char *message, int timeout)
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
void CLSvimsIF::SetRcvTOMinfo(int length, BYTE code, char *message)
{
	RCV_TOM_INFO *ptr;

	switch (code)
	{
	case VIMS_AUTHEN:	ptr = &m_rTomInfo[OP_AUTHEN];
		break;
	case VIMS_LOGIN:	ptr = &m_rTomInfo[OP_LOGIN];
		break;
	case VIMS_LOGOUT:	ptr = &m_rTomInfo[OP_LOGOUT];
		break;
	}

	ptr->sequence = m_rxSequence;
	ptr->length = length;
	memcpy(&ptr->message, message, length);
	gettimeofday(&ptr->rxTime, NULL);
	ptr->waiting = true;		// 송신할 메시지가 있음 (ACK or NACK)
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
bool CLSvimsIF::Manage(void)
{
	time(&m_curClock);		// 현재 시각 갱신
	m_curTod = localtime(&m_curClock);
		
							// Manage RX Timeout
	if (!ManageTimeout())
		return (true);

	// Manage RX
	if (!ManageRX())
	{
		//Log.Write(m_id, 1, "%s RX fail", m_stamp);
		Log.Write(1, "%s RX fail[%d]", m_stamp, m_id);
		Close();
		return (false);
	}
	// Manage TX
	if (!ManageTX())
	{
		//Log.Write(m_id, 1, "%s TX fail", m_stamp);
		Log.Write(1, "%s TX fail[%d]", m_stamp, m_id);
		Close();
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// SetID
//------------------------------------------------------------------------------
void CLSvimsIF::SetID(int id, CLSequip *pEquip)
{
	m_id = id;
	m_pEquip = pEquip;
	sprintf(m_stamp, "CLTCP%04d", m_id);
}
//------------------------------------------------------------------------------
// PrcHeartbeat
//------------------------------------------------------------------------------
void CLSvimsIF::PrcHeartbeat(void)
{

}
//------------------------------------------------------------------------------
// PrcAuthen
//------------------------------------------------------------------------------
void CLSvimsIF::PrcAuthen(void)
{
	//if (AuthenOK())
	//return (true);
	//else return (false);
	
}
//------------------------------------------------------------------------------
// SendConfig
//------------------------------------------------------------------------------
bool CLSvimsIF::SendConfig(void)
{
	char info[VIMS_SHORTBUF_LEN];
	int idx = 0;

	Log.Write(m_id, 1, "%s Send config info", m_stamp);
	memset(info, 0, 147);
	snprintf(info, 16, "127.0.0.1");		idx += 16;
	SetNumber(&info[idx], 5010);			idx += 2;
	info[idx] = 1;						idx += 1;	// ftp 접속 모드
	snprintf(&info[idx], 16, "miosftp");	idx += 16;	// ftp id
	snprintf(&info[idx], 16, "mios4321#"); idx += 16;	// ftp passwd
	snprintf(&info[idx], 32, "/ISPT_IMG");	idx += 32;
	snprintf(&info[idx], 32, "/ISPT_PIC");	idx += 32;
	snprintf(&info[idx], 32, "/ISPT_IMG/SPECIMG");	idx += 32;

	return (SendMessage(VIMS_CONFIG_RES, idx, info));
}
//------------------------------------------------------------------------------
// SendVerInfo
//------------------------------------------------------------------------------
bool CLSvimsIF::SendVerInfo(void)
{
	//1464327460
	char info[VIMS_SHORTBUF_LEN];
	int idx = 0;

	Log.Write(m_id, 1, "%s Send Ver info", m_stamp);
	memset(info, 0, 72);
	SetNumber(info, 1464327460, 4);				idx += 4;
	snprintf(&info[idx], 32, "ISPT_CRTR.CSV");	idx += 32;
	SetNumber(&info[idx], 1464327460, 4);		idx += 4;
	snprintf(&info[idx], 32, "VH_SPEC.CSV");	idx += 32;

	return (SendMessage(VIMS_VER_RES, idx, info));
}
//------------------------------------------------------------------------------
// SendRcptInfo
//------------------------------------------------------------------------------
bool CLSvimsIF::SendRcptInfo(void)
{

}
//------------------------------------------------------------------------------
// SendVhInfo
//------------------------------------------------------------------------------
bool CLSvimsIF::SendVhInfo(void)
{

}
//------------------------------------------------------------------------------
// SendSpecInfo
//------------------------------------------------------------------------------
bool CLSvimsIF::SendSpecInfo(void)
{

}
//------------------------------------------------------------------------------
// SendHeartbeat
//------------------------------------------------------------------------------
bool CLSvimsIF::SendHeartbeat(void)
{

}
//------------------------------------------------------------------------------
// SendAck
//------------------------------------------------------------------------------
bool CLSvimsIF::SendAck(BYTE code, BYTE nackCode)
{
	char info[SHORTBUF_LEN];
	RCV_TOM_INFO *ptr;
	int idx = 0;

	memset(info, 0, 3);

	switch (code)
	{
	case VIMS_AUTHEN:	ptr = &m_rTomInfo[OP_AUTHEN];
		break;
	case VIMS_LOGIN:	ptr = &m_rTomInfo[OP_LOGIN];
		break;
	case VIMS_LOGOUT:	ptr = &m_rTomInfo[OP_LOGOUT];
		break;
	}
		
	info[idx] = ptr->code;					idx += 1;
	SetNumber(&info[idx], ptr->sequence);	idx += 2;
	
	return (SendMessage(VIMS_ACK, idx, info));
}
//------------------------------------------------------------------------------
// SendNAck
//------------------------------------------------------------------------------
bool CLSvimsIF::SendNAck(BYTE code)
{
	char info[SHORTBUF_LEN];
	RCV_TOM_INFO *ptr;
	int idx = 0;
	memset(info, 0, 4);

	switch (code)
	{
	case VIMS_AUTHEN:	ptr = &m_rTomInfo[OP_AUTHEN];
		break;
	case VIMS_LOGIN:	ptr = &m_rTomInfo[OP_LOGIN];
		break;
	case VIMS_LOGOUT:	ptr = &m_rTomInfo[OP_LOGOUT];
		break;
	}

	info[idx] = ptr->code;					idx += 1;
	SetNumber(&info[idx], ptr->sequence);	idx += 2;
	info[idx] = 0x01;						idx += 1;

	return (SendMessage(VIMS_NACK, idx, info));
}
//------------------------------------------------------------------------------
// SendMessage
//------------------------------------------------------------------------------
bool CLSvimsIF::SendMessage(void)
{
	/* 미수신 메시지 재전송 */
	int level;
	char stamp[SHORTBUF_LEN];
	TOM_INFO *ptr = &m_tomInfo;

	if (!Write(ptr->message, ptr->length))
		return (false);

	sprintf(stamp, "%s TX (re)", m_stamp);
	Log.FLdump(m_id, 1, stamp, ptr->message, ptr->length, ptr->length);
	return (true);
}
//------------------------------------------------------------------------------
bool CLSvimsIF::SendMessage(BYTE code, int length, char *info)
{
	int level, txLength = length + VIMS_HEADER_LEN;
	bool waitResponse = false;
	char message[VIMS_TCPBUF_LEN], stamp[SHORTBUF_LEN];

	// General message packet
	memset(message, 0, VIMS_TCPBUF_LEN);
	message[VIMS_OPCODE] = code;
	SetNumber(&message[VIMS_DEV_ID], m_id, 4);

	switch (code)
	{
	case VIMS_ACK:
	case VIMS_NACK:
	case VIMS_CONFIG_RES:
	case VIMS_VER_RES:
	case VIMS_RCPT_RES:
	case VIMS_VH_RES:
	case VIMS_SPEC_RES:
		SetNumber(&message[VIMS_SEQ], ++m_txSequence);
		if (length && info != NULL)
			memcpy(&message[VIMS_DATA], info, length);
		break;
	case VIMS_HEART_REQ:
		waitResponse = true;
		SetNumber(&message[VIMS_SEQ], ++m_txSequence);
		if (length && info != NULL)
			memcpy(&message[VIMS_DATA], info, length);
		break;
	default:
		Log.Write("m_id", 1, "Undefined TX code[%02x]",code);

	}
	SetNumber(&message[VIMS_LEN], txLength);
	if (!Write(message, txLength))
		return (false);
	
	if (waitResponse)
		SetTOMinfo(txLength, code, message);

	sprintf(stamp, "%s TX", m_stamp);
	Log.FLdump(m_id, 1, stamp, message, txLength, txLength);
	return (true);
}
