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

    fread(buf, 1, (size_t) sz, f);
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

    printf("[+] %s: %zu bytes\n", argv[1], sz);
    printf(" primeros 8: ");
    for (int i = 0; i < 8 && i < (int) sz; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    free(data);
    return 0;
}
