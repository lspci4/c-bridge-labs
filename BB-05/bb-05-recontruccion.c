#include <stdio.h>
#include <windows.h>
#include <winnt.h>

// clang-format off
static const BYTE NOOP_SC[] = {
    0x55,             /* push rbp       */
    0x48, 0x89, 0xE5, /* mov rbp, rsp   */
    0x48, 0x31, 0xC0, /* xor rax, rax   */
    0x5D,             /* pop rbp        */
    0xC3              /* ret            */
};
// clang-format on

void run_sellcode(const BYTE *sc, SIZE_T size) {
    /* 1. Reservar espacio en memoria */
    PVOID page = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!page) {
        fprintf(stderr, "VirtualAlloc fallo\n");
        return;
    }
    printf("direccion de memoria: %p\n", page);

    /* 2. Escribir los shellcode en memoria usando memcpy */
    memcpy(page, sc, size);
    FlushInstructionCache(GetCurrentProcess(), page, size);

    /* 3. Ejecutar los shellcode, creo un hilo usando CreateThread  */
    HANDLE ht = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) page, NULL, 0, NULL);
    if (!ht) {
        VirtualFree(page, 0, MEM_RELEASE);
        return;
    }

    /* 4. Esperar a que termine de ejcutarse el hilo */

    DWORD inicio = GetTickCount();
    DWORD w = WaitForSingleObject(ht, 3000); /* Parte 1 Esperar a que termine */
    DWORD fin = GetTickCount();
    printf("[*] tiempo de ejcucion: %lu\n", fin - inicio);

    if (w == WAIT_TIMEOUT) { /* Parte 2 Manejar el timeout */
        printf("[!] timeout\n");
        TerminateThread(ht, 1);
    }

    DWORD exit_code = 0; /* Parte 5 verifio si el shellcode funciono (si ret ) */
    GetExitCodeThread(ht, &exit_code);
    printf("[*] exit code del hilo: %lu\n", exit_code);

    CloseHandle(ht);                   /* Parte 4 Limpiar */
    VirtualFree(page, 0, MEM_RELEASE); /*                 */
}

int main(void) {
    run_sellcode(NOOP_SC, sizeof(NOOP_SC));
    return 0;
}
