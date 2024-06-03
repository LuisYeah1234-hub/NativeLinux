#define WIN32_NO_STATUS
#define NTOS_MODE_USER
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include <stddef.h>
#include <stdarg.h>
#include <excpt.h>
#include <windef.h>
#include <winnt.h>
#include <ntndk.h>
#include <ntddkbd.h>

//
// Device type for input/output
//
typedef enum _CON_DEVICE_TYPE
{
    KeyboardType,
    MouseType
} CON_DEVICE_TYPE;

//
// Display functions
//
NTSTATUS
__cdecl
RtlCliDisplayString(
    IN PCH Message,
    ...
);

NTSTATUS
RtlCliPrintString(
    IN PUNICODE_STRING Message
);

NTSTATUS
RtlCliPutChar(
    IN WCHAR Char
);

//
// Input functions
//
NTSTATUS
RtlCliOpenInputDevice(
    OUT PHANDLE Handle,
    IN CON_DEVICE_TYPE Type
);

//
// Power functions
//
NTSTATUS
RtlCliShutdown(
    VOID
);

NTSTATUS
RtlCliReboot(
    VOID
);

// Keyboard:
HANDLE hKeyboard;

typedef struct _KBD_RECORD {
  WORD  wVirtualScanCode;
  DWORD dwControlKeyState;
  UCHAR AsciiChar;
  BOOL  bKeyDown;
} KBD_RECORD, *PKBD_RECORD;

void IntTranslateKey(PKEYBOARD_INPUT_DATA InputData, KBD_RECORD *kbd_rec);

#define RIGHT_ALT_PRESSED     0x0001 // the right alt key is pressed.
#define LEFT_ALT_PRESSED      0x0002 // the left alt key is pressed.
#define RIGHT_CTRL_PRESSED    0x0004 // the right ctrl key is pressed.
#define LEFT_CTRL_PRESSED     0x0008 // the left ctrl key is pressed.
#define SHIFT_PRESSED         0x0010 // the shift key is pressed.
#define NUMLOCK_ON            0x0020 // the numlock light is on.
#define SCROLLLOCK_ON         0x0040 // the scrolllock light is on.
#define CAPSLOCK_ON           0x0080 // the capslock light is on.
#define ENHANCED_KEY          0x0100 // the key is enhanced.

// Misc
void FillUnicodeStringWithAnsi(OUT PUNICODE_STRING us, IN PCHAR as);

BOOLEAN SetUnicodeString (
  UNICODE_STRING* pustrRet, 
  WCHAR* pwszData
  );
BOOLEAN 
  DisplayString (
  WCHAR* pwszData
  );
BOOLEAN 
  AppendString(
  WCHAR* pszInput, 
  WCHAR* pszAppend
  );
UINT 
  GetStringLength(
  WCHAR* pszInput
  );
