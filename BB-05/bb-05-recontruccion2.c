#include <stdio.h>
#include <windows.h>

// clang-format off
static const BYTE NOOP_SC[] = {
    0x55,             /* push rbp       */
    0x48, 0x89, 0xE5, /* mov rbp, rsp   */
    0x48, 0x31, 0xC0, /* xor rax, rax   */
    0x5D,             /* pop rbp        */
    0xC3              /* ret            */
};
// clang-format on

void run_shellcode(const BYTE *sc, SIZE_T size) {
    // 1. Reservar memoria
    PVOID page = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!page) {
        fprintf(stderr, "VirtualAlloc fallo\n");
        return;
    }

    // 2. Insertar el shellcode en espacio de memorai Reservar
    memcpy(page, sc, size);
    FlushInstructionCache(GetCurrentProcess(), sc, size);

    // 3. Ejecutar el shellcode
    HANDLE ht = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE) page, NULL, 0, NULL);
    if (!ht) {
        VirtualFree(page, 0, MEM_RELEASE);
        return;
    }

    // 4. Esperar a que se ejecute cada hilo
    DWORD inicio = GetTickCount();
    DWORD w = WaitForSingleObject(ht, 3000);
    DWORD fin = GetTickCount();
    printf("tiempo de ejecuion: %lu\n", fin - inicio);

    if (w == WAIT_TIMEOUT) {
        printf("[!] timeout\n");
        TerminateThread(ht, 1);
    }

    DWORD exit_code = 0;
    GetExitCodeThread(ht, &exit_code);
    printf("[*] exit code del hilo: %lu\n", exit_code);

    CloseHandle(ht);
    VirtualFree(page, 0, MEM_RELEASE);
}

int main(void) {
    run_shellcode(NOOP_SC, sizeof(NOOP_SC));
    return 0;
}
