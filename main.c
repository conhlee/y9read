#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include "files.h"

#include "common.h"

typedef struct __attribute((packed)) {
    u32 overlayId;
    u32 relocateAddr;
    u32 relocateSize;
    u32 bssSize;
    u32 siStart; // Static initializer start addr
    u32 siEnd; // Static initializer end addr
    u32 fileId;
    u32 _reserved;
} Y9OverlayEntry;

#define T_OVERLAY_ID      "Overlay ID"
#define T_RELOCATE_ADDR   "Relocation Address"
#define T_RELOCATE_SIZE   "Relocation Size"
#define T_BSS_SIZE        "BSS Size"
#define T_SI_START        "SI Start Addr."
#define T_SI_END          "SI End Addr."
#define T_FILE_ID         "File ID"
#define T_RESERVED        "Reserved"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: y9read <input_bin> [-mo] [--machine-order]\n");
        printf("  <input_bin>    The overlay table binary to read (y9.bin).\n");
        printf("  -mo, --machine-order  Log addresses in machine order (little-endian).\n");

        return 1;
    }

    int byteSwap = 0;
    for (int i = 2; i < argc; i++) {
        if (strcasecmp(argv[i], "-mo") == 0 || strcasecmp(argv[i], "--machine-order") == 0) {
            byteSwap = 1;
            break;
        }
    }

    FileHandle hndl = FileCreateHandle(argv[1]);

    Y9OverlayEntry* entriesStart = (Y9OverlayEntry*)hndl.data_void;
    Y9OverlayEntry* entriesEnd = entriesStart + (hndl.size / sizeof(Y9OverlayEntry));

    printf("Reading overlay table at '%s' (%u overlays):\n\n", argv[1], (unsigned)(entriesEnd - entriesStart));

    printf(
        "%s | %s | %s | %s | %s | %s | %s | %s\n",
        T_OVERLAY_ID, T_RELOCATE_ADDR, T_RELOCATE_SIZE, T_BSS_SIZE, T_SI_START, T_SI_END, T_FILE_ID, T_RESERVED
    );

    Y9OverlayEntry* currentEntry;
    for (currentEntry = entriesStart; currentEntry < entriesEnd; currentEntry++) {
        u32 relocateAddr = byteSwap ?
            __builtin_bswap32(currentEntry->relocateAddr) : currentEntry->relocateAddr;
        u32 siStart = byteSwap ?
            __builtin_bswap32(currentEntry->siStart) : currentEntry->siStart;
        u32 siEnd = byteSwap ?
            __builtin_bswap32(currentEntry->siEnd) : currentEntry->siEnd;

        printf(
            "%*u | %*s0x%08x | %*u | %*u | %*s0x%08x | %*s0x%08x | %*u | %*u\n",
            STR_LIT_LEN(T_OVERLAY_ID), currentEntry->overlayId,
            STR_LIT_LEN(T_RELOCATE_ADDR) - 10, "", relocateAddr,
            STR_LIT_LEN(T_RELOCATE_SIZE), currentEntry->relocateSize,
            STR_LIT_LEN(T_BSS_SIZE), currentEntry->bssSize,
            STR_LIT_LEN(T_SI_START) - 10, "", siStart,
            STR_LIT_LEN(T_SI_END) - 10, "", siEnd,
            STR_LIT_LEN(T_FILE_ID), currentEntry->fileId,
            STR_LIT_LEN(T_RESERVED), currentEntry->_reserved
        );
    }

    FileDestroyHandle(hndl);

    return 0;
}