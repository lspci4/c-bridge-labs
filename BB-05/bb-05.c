#include <stdint.h>
#include <stdio.h>
#include <windows.h>

/* shellcode minimal: solo retorna */
// clang-format off
static const uint8_t NOOP_SC[] = {
    0x55,             /* push rbp       */
    0x48, 0x89, 0xE5, /* mov rbp, rsp   */
    0x48, 0x31, 0xC0, /* xor rax, rax   */
    0x5D,             /* pop rbp        */
    0xC3              /* ret            */
};

// clang-format on

void run_shellcode(const BYTE *sc, SIZE_T size) {
    /* 1. Allocar pagina ejecutable */
    PVOID page = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!page) {
        fprintf(stderr, "VirtualAlloc fallo \n");
        return;
    }
    printf("[*] pagina RWX en: %p\n", page);

    /* 2. Copiar shellcode */
    memcpy(page, sc, size);
    FlushInstructionCache(GetCurrentProcess(), page, size);

    /* 3. Ejecutar en hilo separado */
    HANDLE ht = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) page, NULL, 0, NULL);
    if (!ht) {
        VirtualFree(page, 0, MEM_RELEASE);
        return;
    }
    /* 4. Esperar y limpiar */
    DWORD inicio = GetTickCount();
    DWORD w = WaitForSingleObject(ht, 3000);
    DWORD fin = GetTickCount();
    printf("[*] tiempo de ejecucion: %lu ms\n", fin - inicio);
    if (w == WAIT_TIMEOUT) {
        printf("[!] timeout \n");
        TerminateThread(ht, 1);
    }
    DWORD exit_code = 0;
    GetExitCodeThread(ht, &exit_code);
    printf("[*] exit code del hilo: %lu\n", exit_code);

    CloseHandle(ht);
    VirtualFree(page, 0, MEM_RELEASE);
}

int main(void) {
    printf("[*] ejecutando NOOP_SC (%zu bytes)\n", sizeof(NOOP_SC));
    run_shellcode(NOOP_SC, sizeof(NOOP_SC));
    printf("[+] shellcode termino sin crash \n");
    return 0;
}
