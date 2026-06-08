/* contar null bytes */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* shellcode sucio */
// clang-format off
static const uint8_t SUCIO_SC[] = {
    0x55,             /* push rbp       */
    0x48, 0x89, 0xE5, /* mov rbp, rsp   */
    0x48, 0x31, 0xC0, /* xor rax, rax   */
    0x00, 0x00, 0x00,
    0x5D,             /* pop rbp        */
    0xC3
};

static const uint8_t LIMPIO_SC[] = {
    0x55,        /* push rbp */
    0x5D,        /* pop rbp  */
    0xC3         /* ret      */
};
// clang-format on

void analizar(const uint8_t *sc, size_t sz) {
    int nulls = 0;
    for (size_t i = 0; i < sz; i++) {
        if (sc[i] == 0x00) {
            nulls++;
        }
    }
    printf("[*] cantidad de bytes: %d, bytes nulls encontrados: %zu.\n", (int) sz, nulls);

    for (size_t i = 0; i < 8 && i < sz; i++) {
        printf("%02X ", sc[i]);
    }
    printf("\n");
}

int main(void) {
    analizar(SUCIO_SC, sizeof(SUCIO_SC));
    analizar(LIMPIO_SC, sizeof(LIMPIO_SC));

    return 0;
}
