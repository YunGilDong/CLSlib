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

	// ���Ŵ�� ����Ȯ��
	if (!ptr->waiting)
		return (false);
	// �Ϸù�ȣ Ȯ��
	if (ptr->sequence != sequence)
		return (false);
	// ������� ����Ȯ��
	switch (ptr->code) // send message code
	{
	case VIMS_HEART_REQ:
		if (code != VIMS_HEART_RES)
			return (false);
		break;		// �۽ſ� ���� ���� Ȯ��
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

	// ���� ���� Ȯ��
	if (Connected)
		return (CON_OK);
	// �ʱ�ȭ ���� Ȯ��
	if (!Initialized && !Open())
		return (CON_ERROR);
	/*
	// ���� �䱸
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

	// ���Ÿ޽��� Ȯ��
	m_rxSequence = GetNumber(m_message[VIMS_SEQ1]);
	sprintf(stamp, "%s RX", m_stamp);
	Log.FLdump(m_id, 1, stamp, m_message, m_tally, m_tally);

	waitChecked = CheckTOMinfo();

	// ���Ÿ޽��� ó��
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
bool CLSvimsIFH::ManageTimeout(void)
{
	TOM_INFO *ptr = &m_tomInfo;

	// ���Ŵ�� ���� Ȯ��
	if (!ptr->waiting)	// waiting message�� ������
		return (true);

	// ���ð� ��� ���� Ȯ��
	if (!CheckElapsedTime(&ptr->txTime, ptr->timeout))	// ���ð��� ������ Ÿ�Ӿƿ� �ð� �̳���
		return (true);

	// Timeout �̻� ó�� ���� ����
	Log.Write((m_id, 2, "%s Timeout %d/%d [%02x]", m_stamp, ptr->count, VIMS_TIMEOUT_MAX, ptr->code);
	if (++ptr->count >= VIMS_TIMEOUT_MAX)
	{
		ptr->waiting = false;
		return (true);
	}

	// ���� ������
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

	// ���Ŵ�� ���� Ȯ��
	if (ptr->waiting)
		return (true);

	// ���� ���� Ȯ��

	// �α��� Ȯ��

	// �������� ���� Ȯ��

	// �������� ���� Ȯ��

	// �������� ��û Ȯ��

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
	ptr->waiting = true;		// ���� ���� �޼����� ����.
}
void CLSvimsIFH::SetRcvTOMinfo(int length, BYTE code, char *message)
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
	ptr->waiting = true;		// �۽��� �޽����� ���� (ACK or NACK)
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
bool CLSvimsIFH::Manage(void)
{
	time(&m_curClock);		// ���� �ð� ����
	m_curTod = localtime(&m_curClock);

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
//------------------------------------------------------------------------------
// PrcAuthen
//------------------------------------------------------------------------------
void CLSvimsIFH::PrcAuthen(void)
{
	//if (AuthenOK())
	//return (true);
	//else return (false);
	
}
//------------------------------------------------------------------------------
// SendConfig
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendConfig(void)
{
	char info[VIMS_SHORTBUF_LEN];
	int idx = 0;

	Log.Write(m_id, 1, "%s Send config info", m_stamp);
	memset(info, 0, 147);
	snprintf(info, 16, "127.0.0.1");		idx += 16;
	SetNumber(info[idx], 5010);			idx += 2;
	info[idx] = 1;						idx += 1;	// ftp ���� ���
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
bool CLSvimsIFH::SendVerInfo(void)
{
	//1464327460
	char info[VIMS_SHORTBUF_LEN];
	int idx = 0;

	Log.Write(m_id, 1, "%s Send Ver info", m_stamp);
	memset(info, 0, 72);
	SetNumber(info, 1464327460, 4);				idx += 4;
	snprintf(&info[idx], "ISPT_CRTR.CSV", 32);	idx += 32;
	SetNumber(&info[idx], 1464327460, 4);		idx += 4;
	snprintf(&info[idx], "VH_SPEC.CSV", 32);	idx += 32;

	return (SendMessage(VIMS_VER_RES, idx, info));
}
//------------------------------------------------------------------------------
// SendRcptInfo
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendRcptInfo(void)
{

}
//------------------------------------------------------------------------------
// SendVhInfo
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendVhInfo(void)
{

}
//------------------------------------------------------------------------------
// SendSpecInfo
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendSpecInfo(void)
{

}
//------------------------------------------------------------------------------
// SendHeartbeat
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendHeartbeat(void)
{

}
//------------------------------------------------------------------------------
// SendAck
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendAck(BYTE code, BYTE nackCode)
{
	char info[SHORTBUF_LEN];
	RCV_TOM_INFO *ptr;
	int idx = 0;

	memset(info, 0, 3);

	switch (code)
	{
	case OP_AUTHEN:	ptr = &m_rTomInfo[OP_AUTHEN];
		break;
	case OP_LOGIN:	ptr = &m_rTomInfo[OP_LOGIN];
		break;
	case OP_LOGOUT:	ptr = &m_rTomInfo[OP_LOGOUT];
		break;
	}
		
	info[idx] = ptr->code;					idx += 1;
	SetNumber(&info[idx], ptr->sequence);	idx += 2;
	
	return (SendMessage(VIMS_ACK, idx, info));
}
//------------------------------------------------------------------------------
// SendNAck
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendNAck(void)
{
	char info[SHORTBUF_LEN];
	RCV_TOM_INFO *ptr;
	int idx = 0;

	memset(info, 0, 4);

	switch (code)
	{
	case OP_AUTHEN:	ptr = &m_rTomInfo[OP_AUTHEN];
		break;
	case OP_LOGIN:	ptr = &m_rTomInfo[OP_LOGIN];
		break;
	case OP_LOGOUT:	ptr = &m_rTomInfo[OP_LOGOUT];
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
bool CLSvimsIFH::SendMessage(void)
{
	/* �̼��� �޽��� ������ */
	int level;
	char stamp[SHORTBUF_LEN];
	TOM_INFO *ptr = &m_tomInfo;

	if (!Write(ptr->message, ptr->length))
		return (false);

	sprintf(stamp, "%s TX (re)", m_stamp);
	Log.FLdump(m_id, 1, "stamp, ptr->message, ptr->length, ptr->length");
	return (true);
}
//------------------------------------------------------------------------------
bool CLSvimsIFH::SendMessage(BYTE code, int length, char *info)
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
		Log.Write("m_id", 1, "Undefined TX code[%02x]"code);

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