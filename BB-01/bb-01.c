#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* Escribir un valor de 64 bits en buf[offset] */
void write64(uint8_t *buf, size_t offset, uint64_t val) {
    *(uint64_t *) (buf + offset) = val;
}

/* Leer un valor de 64 bits desde buf[offset] */
uint64_t read64(const uint8_t *buf, size_t offset) {
    return *(const uint64_t *) (buf + offset);
}

/* Uso: */
int main(void) {
    uint8_t buf[64] = {0};
    write64(buf, 0, 0xDEADBEEFCAFEBABE);
    write64(buf, 8, 0x0000000140001234);
    write64(buf, 16, 0x0000000140005678);

    printf("buf[0..7] = 0x%016llX\n", read64(buf, 0));
    printf("buf[8..15] = 0x%016llX\n", read64(buf, 8));

    return 0;
}
