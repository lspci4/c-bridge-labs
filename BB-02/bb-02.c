/* Llamar una función a través de un puntero */

#include <stdio.h>
void accion_normal(void) {
    printf("accion_normal ejecutada\n");
}

void accion_maliciosa(void) {
    printf("accion_maliciosa ejecutada\n");
}

int main(void) {
    /* llamada a la funcion normalmente */
    accion_normal();

    /* Un puntero a función que no recibe nada y no retorna nada */
    void (*fn)(void) = accion_normal;
    fn();

    /* Reasignar el puntero (lo que hace el exploit UAF) */
    fn = accion_maliciosa;
    fn();

    /* Castear un void* a función ( lo que hace shellcode_test.c) */
    void *ptr = (void *) accion_normal;
    void (*fn2)(void) = (void (*)(void)) ptr;
    fn2();

    return 0;
}
