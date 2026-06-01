#define _CRT_SECURE_NO_WARNINGS
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Retorna un buffer con el contenido del archivo.
 *out_size recibe el tamaño. El caller deber llamar free(). */
unsigned char *read_file(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror(path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);

    unsigned char *buf = malloc((size_t) sz);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    /* Modificación A — verificar la lectura completa */
    size_t leidos = fread(buf, 1, (size_t) sz, f);
    if (leidos != (size_t) sz) {
        fprintf(stderr, "lectura incompleta: %zu de %zu bytes\n", leidos, (size_t) sz);
        free(buf);
        fclose(f);
        return NULL;
    }

    fclose(f);
    *out_size = (size_t) sz;
    return buf;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("uso: prog archivo\n");
        return 1;
    }

    size_t sz = 0;
    unsigned char *data = read_file(argv[1], &sz);
    if (!data) {
        return 1;
    }

    /* Modificación B — imprimir más bytes
        Cambia el loop que imprime los primeros 8 bytes para que imprima los primeros 16 bytes,
        con un salto de línea cada 8 (para que sea legible).
        */
    printf("[+] %s: %zu bytes\n", argv[1], sz);
    printf(" primeros 16: \n");
    for (int i = 0; i < 16 && i < (int) sz; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    /* Modificación C — agregar un dump de los últimos 8 bytes
        Después del loop de los primeros bytes, agrega otro que imprima los últimos 8 bytes del
       archivo. Esto te obliga a indexar data[sz - 8] hasta data[sz - 1].
    */
    printf("ultimos 8 bytes: \n");
    int inicio = (int) sz - 8;
    if (inicio < 0) {
        inicio = 0;
    }
    for (int i = inicio; i < (int) sz; i++) {
        printf("%02X ", data[i]);
    }

    free(data);
    return 0;
}
