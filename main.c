#include "precomp.h"
#include "BootImage.h"

HANDLE hKeyboard;

#define BOOT_IMAGE_SIZE  3476752

BOOLEAN Vt100Parsing = FALSE;
KEYBOARD_INPUT_DATA g_KeyboardData;

void mini_rv32ima_putchar(char c) {
    char string[2] = { c, 0 };

	if (c == '\033') {
        // We ignore any type of ANSI as they aren't supported in Native
		Vt100Parsing = TRUE;
	}

	if (Vt100Parsing) {
		if (c == 'm' || c == 'A' || c == 'B' || c == 'C' || c == 'D') Vt100Parsing = FALSE;
		return;
	}
 
    RtlCliDisplayString(string);
}

void mini_rv32ima_minisleep() {
    // To prevent mass text from crashing the program we will do a small delay
    LARGE_INTEGER delay;
    memset(&delay, 0x00, sizeof(LARGE_INTEGER));
    delay.LowPart = 100000000;
    NtDelayExecution(FALSE, &delay); // Small delay to prevent flooding
}

void mini_rv32ima_print(char* string) {
    DbgPrint("RV32: %s\n", string);
}

int mini_rv32ima_key_hit(void) {
    IO_STATUS_BLOCK Iosb;
    LARGE_INTEGER ByteOffset;
    NTSTATUS Status;

    // Clean up memory
    RtlZeroMemory(&Iosb, sizeof(Iosb));
    RtlZeroMemory(&ByteOffset, sizeof(ByteOffset));
    RtlZeroMemory(&g_KeyboardData, sizeof(KEYBOARD_INPUT_DATA));

    // Try to read the data synchronously
    Status = NtReadFile(hKeyboard, NULL, NULL, NULL, &Iosb, &g_KeyboardData, sizeof(KEYBOARD_INPUT_DATA), &ByteOffset, NULL);

    if (NT_SUCCESS(Status) && Iosb.Information == sizeof(KEYBOARD_INPUT_DATA)) {
        // A key hit has been detected, return.
        return 1;
    } else if (Status == STATUS_PENDING) {
        // No input to read at the moment, cancel read and return.
        NtCancelIoFile(hKeyboard, &Iosb);
        return 0;
    } else {
        return 0;
    }
}

int mini_rv32ima_get_key(void) {
    KBD_RECORD kbd_rec;

    IntTranslateKey(&g_KeyboardData, &kbd_rec);

    if (!kbd_rec.bKeyDown)
    {
        return (-1);
    }
    return kbd_rec.AsciiChar;
}


void* mini_rv32ima_malloc(size_t count) {
	return RtlAllocateHeap(RtlGetProcessHeap(), HEAP_ZERO_MEMORY, count);
}

uint32_t mini_rv32ima_handle_control_store(uint32_t addy, uint32_t val) {
    if (addy == 0x10000000) mini_rv32ima_putchar(val);

    return 0;
}

uint32_t mini_rv32ima_handle_control_load(uint32_t addy) {
	if (addy == 0x10000005) return 0x60 | mini_rv32ima_key_hit();
	return 0;
}

void mini_rv32ima_handle_other_csr_write(uint16_t csrno, uint32_t value) {
    if (csrno == 0x139) mini_rv32ima_putchar(value);
}

uint32_t mini_rv32ima_handle_other_csr_read(uint16_t csrno) {
	if (csrno == 0x140) {
		if (!mini_rv32ima_key_hit()) return -1;
		return mini_rv32ima_get_key();
	}
	return 0;
}

#define RAM_SIZE 64 * 1024 * 1024

#define MINIRV32WARN
#define MINI_RV32_RAM_SIZE RAM_SIZE
#define MINIRV32_DECORATE static
#define MINIRV32_IMPLEMENTATION
#define MINIRV32_HANDLE_MEM_STORE_CONTROL(addy, val) if(mini_rv32ima_handle_control_store( addy, val )) return val;
#define MINIRV32_HANDLE_MEM_LOAD_CONTROL(addy, rval) rval = mini_rv32ima_handle_control_load( addy );
#define MINIRV32_OTHERCSR_WRITE(csrno, value) mini_rv32ima_handle_other_csr_write(csrno, value);
#define MINIRV32_OTHERCSR_READ(csrno, value) value = mini_rv32ima_handle_other_csr_read(csrno);

#include "mini-rv32ima.h"
#include "default64mbdtc.h"

UCHAR* memory;
struct MiniRV32IMAState* cpu = NULL;

NTSTATUS main() {
    PPEB Peb = NtCurrentPeb();
    NTSTATUS Status;
    BOOLEAN running = 0;
    BOOLEAN reboot = 0;
    uint32_t* dtb;
    uint32_t validram;
    ULONG_PTR dtbPtr;

    // Setup keyboard input
    Status = RtlCliOpenInputDevice(&hKeyboard, KeyboardType);

    memory = mini_rv32ima_malloc(RAM_SIZE);
    RtlZeroMemory(memory, RAM_SIZE);

    RtlCopyMemory(memory, BootImage, BOOT_IMAGE_SIZE);

    dtbPtr = RAM_SIZE - sizeof(default64mbdtb) - sizeof(struct MiniRV32IMAState);
    RtlCopyMemory(memory + dtbPtr, default64mbdtb, sizeof(default64mbdtb));

    cpu = (struct MiniRV32IMAState*)(memory + RAM_SIZE - sizeof(struct MiniRV32IMAState));
    cpu->pc = MINIRV32_RAM_IMAGE_OFFSET;
    cpu->regs[10] = 0x00; //hart ID
    cpu->regs[11] = (dtbPtr + MINIRV32_RAM_IMAGE_OFFSET); //dtb_pa (Must be valid pointer) (Should be pointer to dtb)
    cpu->extraflags |= 3; // Machine-mode.

    dtb = (uint32_t*)(memory + dtbPtr);
    if (dtb[0x13c / 4] == 0x00c0ff03) {
        validram = dtbPtr;
        dtb[0x13c / 4] = (validram >> 24) | (((validram >> 16) & 0xff) << 8) | (((validram >> 8) & 0xff) << 16) | ((validram & 0xff) << 24);
    }

    running = TRUE;
    reboot = FALSE;
    while (running) {
        int ret = MiniRV32IMAStep(cpu, memory, 0, 1, 1024); // Execute up to 1024 cycles before breaking out.

        switch (ret) {
            case 0: break;
            case 1: mini_rv32ima_minisleep(); break;
            case 3: break;
            case 30583: mini_rv32ima_print("reboot requested. restarting\n"); reboot = TRUE; running = FALSE;
            case 21845: mini_rv32ima_print("poweroff requested. shutting down\n"); running = FALSE; 
            default: running = FALSE;
        }
    }

    NtClose(hKeyboard);
    RtlFreeHeap(RtlGetProcessHeap(), 0, memory);

    if (reboot == TRUE) {
        RtlCliReboot();
    } else {
        RtlCliShutdown();
    }

    return STATUS_SUCCESS;
}