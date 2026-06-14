#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPRAY_COUNT 64
#define CHUNK_SIZE 56 /* mismo tamaño que el objeto victima */

int main(void) {
    void *spray[SPRAY_COUNT] = {0};

    // Fase 1: llenar el allocator con chunks del mismo tamaño */
    for (int i = 0; i < SPRAY_COUNT; i++) {
        spray[i] = malloc(CHUNK_SIZE);
        memset(spray[i], 0x41, CHUNK_SIZE); /* relleno conoido */
    }

    // Fase 2: liberar los pares (crear huecos controlados) */
    for (int i = 0; i < SPRAY_COUNT; i += 2) {
        free(spray[i]);
        spray[i] = NULL;
    }

    // Ahora malloc(CHUNK_SIZE) retorna uno de los huecos */
    void *weapon = malloc(CHUNK_SIZE);
    memset(weapon, 0xBB, CHUNK_SIZE); /* contenido controlado */

    printf("[+] weapon @ %p \n", weapon);

    // verificar que weapon reutilizo un hueco del spray */
    // (en un exploit real: weapon es el objeto con el que controlamos RIP ) */

    free(weapon);
    for (int i = 1; i < SPRAY_COUNT; i += 2) {
        free(spray[i]);
    }
    return 0;
}
