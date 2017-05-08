//------------------------------------------------------------------------------
//
// Project:		트램관제시스템 개발
// Target:		General
// Filename:	Define.h
// Version:		1.0
// History:
// 
//------------------------------------------------------------------------------
#ifndef	DefineH
#define	DefineH
//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "General.h"
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// Database
//------------------------------------------------------------------------------
#define	DB_USER				(char *)"tram"
#define	DB_PASSWORD			(char *)"tram"
#define	DB_ALIAS			(char *)"tram"
//------------------------------------------------------------------------------
#define	MAX_APPROACH		4
#define	MAX_BIT				16
#define	MAX_LOCAL			32
#define	MAX_MDT				32
#define	MAX_ROUTE			32
#define	MAX_STOP			32
#define	MAX_STOP_PER_ROUTE	32
#define	MAX_TAG				128
#define	MAX_TAG_PER_ROUTE	32
#define	MAX_TRAM			32
#define	MAX_TSU				32
#define	MAX_TURNOUT			16
//------------------------------------------------------------------------------
#define	MAX_IMAGE			MAX_TSU			// 순서 주의 ... MAX_TSU 뒤에 정의
#define	MAX_LAMP			(MAX_LOCAL*MAX_APPROACH)
#define	MAX_LINK			(MAX_LOCAL*MAX_APPROACH*2)
#define	MAX_RTTAG			(MAX_ROUTE*MAX_TAG_PER_ROUTE)
#define	MAX_RTSTOP			(MAX_ROUTE*MAX_STOP_PER_ROUTE)
//------------------------------------------------------------------------------
// Directory
//------------------------------------------------------------------------------
#define	DIR_HOME				"/home/tram/work"
#define	DIR_LOG					DIR_HOME"/log/"
#define	DIR_BIN					DIR_HOME"/bin/"
//------------------------------------------------------------------------------
// Macro
//------------------------------------------------------------------------------
#define	IN_RANGE(v,a,b)		(((v) >= (a) && (v) <= (b)) ? true : false)
//------------------------------------------------------------------------------
#endif	// DefineH
//------------------------------------------------------------------------------
// End of DefineH.h
//------------------------------------------------------------------------------
