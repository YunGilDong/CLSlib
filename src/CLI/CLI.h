//------------------------------------------------------------------------------
#ifndef CLIH
#define CLIH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "SharedMem.h"
#include "CLScommand.h"
#include "CLSmemory.h"
#include "CLSlog.h"
#include <unistd.h>
#include <getopt.h>

//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
#define MAX_CLICMD				4
#define DBG_LINE_LEN			80
#define TOKEN_LEN				32
//------------------------------------------------------------------------------
#define KEY_ENTER				0x0D
#define KEY_NEWLINE				0x0A
#define KEY_SPACE				0x20
//------------------------------------------------------------------------------
#define EOT_DONTCARE			0
#define EOT_VALID				1
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
typedef struct
{
	char *cmdPtr;
	char command[BUFFER_LEN];
	char token[TOKEN_LEN];
} CLI_INFO;
//------------------------------------------------------------------------------
#endif // CLI.h
//------------------------------------------------------------------------------
// End of CLI.h
//------------------------------------------------------------------------------

