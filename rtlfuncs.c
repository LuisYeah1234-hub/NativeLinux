#include "precomp.h"

HANDLE hEvent;
WCHAR DisplayBuffer[1024];
USHORT LinePos = 0;
WCHAR PutChar[2] = L" ";
UNICODE_STRING CharString = {2, 2, PutChar};

NTSTATUS
__cdecl
RtlCliDisplayString(IN PCH Message, ...)
{
    va_list MessageList;
    PCHAR MessageBuffer;
    UNICODE_STRING MessageString;
    NTSTATUS Status;

    //
    // Allocate Memory for the String Buffer
    //
    MessageBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, 512);

    //
    // First, combine the message
    //
    va_start(MessageList, Message);
    _vsnprintf(MessageBuffer, 512, Message, MessageList);
    va_end(MessageList);

    //
    // Now make it a unicode string
    //
    RtlCreateUnicodeStringFromAsciiz(&MessageString, MessageBuffer);

    //
    // Display it on screen
    //
    Status = RtlCliPrintString(&MessageString);

    //
    // Free Memory
    //
    RtlFreeHeap(RtlGetProcessHeap(), 0, MessageBuffer);
    RtlFreeUnicodeString(&MessageString);

    //
    // Return to the caller
    //
    return Status;
}

NTSTATUS
RtlCliPrintString(IN PUNICODE_STRING Message)
{
    NTSTATUS Status;
    ULONG i;

    //
    // Loop every character
    //
    for (i = 0; i < (Message->Length / sizeof(WCHAR)); i++)
    {
        //
        // Print the character
        //
        Status = RtlCliPutChar(Message->Buffer[i]);
    }

    //
    // Return status
    //
    return Status;
}

NTSTATUS
RtlCliPutChar(IN WCHAR Char)
{
    //
    // Initialize the string
    //
    CharString.Buffer[0] = Char;

    //
    // Check for overflow, or simply update.
    //
#if 0
    if (LinePos++ > 80)
    {
        //
        // We'll be on a new line. Do the math and see how far.
        //
        MessageLength = NewPos - 80;
        LinePos = sizeof(WCHAR);
    }
#endif

    //
    // Make sure that this isn't backspace
    //
    if (Char != '\r')
    {
        //
        // Check if it's a new line
        //
        if (Char == '\n')
        {
            //
            // Reset the display buffer
            //
            LinePos = 0;
            DisplayBuffer[LinePos] = UNICODE_NULL;
        }
        else
        {
            //
            // Add the character in our buffer
            //
            DisplayBuffer[LinePos] = Char;
            LinePos++;
        }
    }

    //
    // Print the character
    //
    return NtDisplayString(&CharString);
}

NTSTATUS
RtlCliOpenInputDevice(OUT PHANDLE Handle,
    IN CON_DEVICE_TYPE Type)
{
    UNICODE_STRING Driver;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK Iosb;
    HANDLE hDriver;
    NTSTATUS Status;

    //
    // Chose the driver to use
    // FIXME: Support MouseType later
    // FIXME: Don't hardcode keyboard path
    //
    if (Type == KeyboardType)
    {
        RtlInitUnicodeString(&Driver, L"\\Device\\KeyboardClass0");
    }

    //
    // Initialize the object attributes
    //
    InitializeObjectAttributes(&ObjectAttributes,
        &Driver,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL);

    //
    // Open a handle to it
    //
    Status = NtCreateFile(&hDriver,
        SYNCHRONIZE | GENERIC_READ | FILE_READ_ATTRIBUTES,
        &ObjectAttributes,
        &Iosb,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_OPEN,
        FILE_DIRECTORY_FILE,
        NULL,
        0);

    //
    // Now create an event that will be used to wait on the device
    //
    InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL);
    Status = NtCreateEvent(&hEvent, EVENT_ALL_ACCESS, &ObjectAttributes, 1, 0);

    //
    // Return the handle
    //
    *Handle = hDriver;
    return Status;
}

NTSTATUS
RtlCliShutdown(VOID)
{
    BOOLEAN Old;

    //
    // Get the shutdown privilege and shutdown the system
    //
    RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &Old);
    return ZwShutdownSystem(ShutdownNoReboot);
}

NTSTATUS
RtlCliReboot(VOID)
{
    BOOLEAN Old;

    //
    // Get the shutdown privilege and shutdown the system
    //
    RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &Old);
    return ZwShutdownSystem(ShutdownReboot);
}