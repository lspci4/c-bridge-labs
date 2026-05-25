#include <stdio.h>

/* Una "vtable" es un struct donde cada campo es un puntero a función */
typedef struct {
    void (*draw)(void);
    void (*resize)(int w, int h);
    void (*destroy)(void);
} Vtable;

typedef struct {
    Vtable(*vt); /* primer campo = puntero a la vtable */
    int x, y;
} Objeto;

void draw_real(void) {
    printf("dibujando\n");
}

void resize_real(int w, int h) {
    printf("resize %d%d\n", w, h);
}

void destroy_real(void) {
    printf("destruyendo\n");
}

/* Fake vtable: lo que el exploit pone en lugar de la real */
void exploit_dev(void) {
    printf("[!] vtable hijacked - ejecutando shellcode\n");
}

int main(void) {
    Vtable vt_real = {draw_real, resize_real, destroy_real};
    Vtable vt_fake = {exploit_dev, NULL, NULL};

    Objeto obj = {&vt_real, 10, 20};

    obj.vt->draw();    /* normal */
    obj.vt = &vt_fake; /* exploit: reemplazar vtable */
    obj.vt->draw();    /* hijacked */

    return 0;
}
