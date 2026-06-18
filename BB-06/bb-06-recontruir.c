/* BB-06: Heap spray — allocar N chunks del mismo tamaño - Reconstrucción */
/* Técnica para manipular el comportamiento de la memoria */
/*
    1. includes
    2. defines (SPRAY_COUNT y CHUNK_SIZE)
    3. FakeVtable y shellcode_simulado
    4. main:
       - declarar el array spray
       - Fase 1: llenar
       - Fase 2: liberar pares + guardar en liberados
       - weapon: malloc + FakeVtable + invocar
       - verificar si reutilizó un hueco
       - cleanup: liberar impares
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPRAY_COUNT 512
#define CHUNK_SIZE 56 /* Bytes por cada elemento */

typedef struct {
    void (*draw)(void);
} FakeVtable;

void shellcode_simulado() {
    printf("[!] Hijacked..\n");
}

int main(void) {
    void *spray[SPRAY_COUNT] = {0};

    /* Fase 1: LLenar */
    for (int i = 0; i < SPRAY_COUNT; i++) {
        spray[i] = malloc(CHUNK_SIZE);
        memset(spray[i], 0x41, CHUNK_SIZE); /* Relleno los bloques de memoria con con 'A' */
    }

    /* Fase 2: liberar pares + guardar en liberados */
    void *liberados[SPRAY_COUNT] = {0};
    for (int i = 0; i < SPRAY_COUNT; i += 2) {
        liberados[i] = spray[i];
        free(spray[i]);
        spray[i] = NULL;
    }

    /* weapon: malloc + FakeVtable + invocar */
    void *weapon = malloc(CHUNK_SIZE);
    memset(weapon, 0x42, CHUNK_SIZE);
    FakeVtable *fvt = (FakeVtable *) weapon;
    fvt->draw = shellcode_simulado;
    fvt->draw();

    /* verificar si reutilizó un hueco */
    int reutilizo = 0;
    for (int i = 0; i < SPRAY_COUNT; i += 2) {
        if (weapon == liberados[i]) {
            printf("[+] weapon reutilizo spray[%d]\n", i);
            reutilizo = 1;
            break;
        }
    }
    if (!reutilizo) {
        printf("[-] weapon no reutilizo ningun hueco\n");
    }
    /* cleanup: liberar impares */
    for (int i = 1; i < SPRAY_COUNT; i += 2) {
        free(spray[i]);
    }
    free(weapon);
    return 0;
}
