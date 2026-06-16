#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPRAY_COUNT 512 /* cantidad de bloques */
#define CHUNK_SIZE 56   /* mismo tamaño que el objeto victima | Tamaño de cada bloque*/

typedef struct {
    void (*draw)(void);
} FakeVtable;

void shellcode_simulado(void) {
    printf("[!] vtable hijacked via heap spray!\n");
}

int main(void) {
    void *spray[SPRAY_COUNT] = {0};

    // Fase 1: llenar el allocator con chunks del mismo tamaño */
    for (int i = 0; i < SPRAY_COUNT; i++) {
        spray[i] = malloc(CHUNK_SIZE);
        memset(spray[i], 0x41, CHUNK_SIZE); /* relleno conoido */
    }
    printf("[*] Fase 1: %d chunks allocados (%d bytes cada uno)\n", SPRAY_COUNT, CHUNK_SIZE);

    void *liberados[SPRAY_COUNT] = {0};
    // Fase 2: liberar los pares (crear huecos controlados) */
    for (int i = 0; i < SPRAY_COUNT; i += 2) {
        liberados[i] = spray[i];
        free(spray[i]);
        spray[i] = NULL;
    }
    printf("[*] Fase 2: %d huecos creados de %d bytes\n", SPRAY_COUNT / 2, CHUNK_SIZE);

    // Ahora malloc(CHUNK_SIZE) retorna uno de los huecos */
    void *weapon = malloc(CHUNK_SIZE);
    memset(weapon, 0xBB, CHUNK_SIZE); /* contenido controlado */
    printf("[+] weapon @ %p \n", weapon);
    printf("[*] weapon allocado en hueco del spray\n");

    FakeVtable *fvt = (FakeVtable *) weapon;
    fvt->draw = shellcode_simulado;
    fvt->draw();

    // verificar que weapon reutilizo un hueco del spray */
    int reutilizo = 0;
    for (int i = 0; i < SPRAY_COUNT; i += 2) {
        if (weapon == liberados[i]) {
            printf("[+] weapon reutilizo spray[%d] @ %p\n", i, liberados[i]);
            reutilizo = 1;
            break;
        }
    }
    if (!reutilizo) {
        printf("[-] weapon NO reutilizo ningun hueco del spray\n");
    }
    // (en un exploit real: weapon es el objeto con el que controlamos RIP ) */

    free(weapon);
    for (int i = 1; i < SPRAY_COUNT; i += 2) {
        free(spray[i]);
    }
    return 0;
}
