#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * BB-02 Ejercicio: Struct con puntero a función + simulación UAF
 * ============================================================ */

typedef struct Objeto {
    void (*execute)(void);
} Objeto;

void accion_normal(void) {
    printf("accion_normal ejecutada\n");
}

void accion_maliciosa(void) {
    printf("accion_maliciosa ejecutada (hijacked!)\n");
}

int main(void) {

    /* ---------- Parte 1: struct directo (en la pila) ---------- */
    printf("===== struct directo, acceso con punto =====\n");

    Objeto obj1;
    Objeto obj2;

    obj1.execute = accion_normal;
    obj2.execute = accion_maliciosa;

    obj1.execute(); /* invoca accion_normal    */
    obj2.execute(); /* invoca accion_maliciosa */

    /* ---------- Parte 2: puntero a struct (en el heap) ---------- */
    printf("\n===== puntero a struct, acceso con flecha =====\n");

    Objeto *ptr1 = malloc(sizeof(Objeto));
    Objeto *ptr2 = malloc(sizeof(Objeto));

    if (ptr1 == NULL || ptr2 == NULL) {
        fprintf(stderr, "malloc fallo\n");
        return 1;
    }

    ptr1->execute = accion_normal;
    ptr2->execute = accion_maliciosa;

    ptr1->execute(); /* invoca accion_normal    */
    ptr2->execute(); /* invoca accion_maliciosa */

    /* ---------- Parte 3: simulacion de UAF (Use-After-Free) ---------- */
    printf("\n===== simulacion UAF =====\n");

    free(ptr1); /* liberamos la memoria  */
    /* a partir de aqui, usar ptr1 es comportamiento indefinido */

    ptr1->execute = accion_maliciosa; /* escribir en memoria liberada */
    ptr1->execute();                  /* invocar lo "inyectado"       */

    /* ---------- Limpieza final ---------- */
    free(ptr2);
    /* NO liberamos ptr1 otra vez: doble free seria otro bug */

    return 0;
}
