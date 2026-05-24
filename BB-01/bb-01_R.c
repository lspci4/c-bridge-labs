#include <stdint.h>
#include <stdio.h>

/* Escribir 8 bytes en buf[offset] */
void write64(uint8_t *buf, size_t offset, uint64_t val) {
    *(uint64_t *) (buf + offset) = val;
}

/* Leer 8 bytes desde buf[offset] */
uint64_t read64(const uint8_t *buf, size_t offset) {
    return *(const uint64_t *) (buf + offset);
}

/* Uso: */
int main(void) {
    uint8_t buf[64] = {0};
    write64(buf, 0, 0xA1B2C3D4E5F67890);
    write64(buf, 8, 0x00000000123456789);
    write64(buf, 16, 0x0000000000001234);

    printf("buf[0..7]  = 0x%016llX\n", read64(buf, 0));
    printf("buf[8..16]  = 0x%016llX\n", read64(buf, 8));
    printf("buf[17..25]  = 0x%016llX\n", read64(buf, 16));
    return 0;
}
