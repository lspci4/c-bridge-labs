#include <stdint.h>
#include <stdio.h>

typedef struct Process {
    uint8_t pad[0x40];
    uint64_t pid;
    struct Process *flink;
    uint8_t pad2[0x68];
    uint64_t token;
} Process;

void steal_token(Process *current, Process *head) {
    Process *p = head;

    do {
        printf("[*] visitando pid=%llu\n", (unsigned long long) p->pid);
        if (p->pid == 4) {
            current->token = p->token;
            printf("[+] token robado de SYSTEM (pid=4) a pid=%llu\n",
                   (unsigned long long) current->pid);
            return;
        }
        p = p->flink;
    } while (p != head);
    {
        printf("[-] SYSTEM no encontrado\n");
    }
}

int main(void) {
    Process system_proc = {.pid = 4, .token = 0xDEADBEEF};
    Process p1 = {.pid = 100, .token = 0x11111111};
    Process p2 = {.pid = 200, .token = 0x22222222};
    Process p3 = {.pid = 300, .token = 0x33333333};
    Process p4 = {.pid = 400, .token = 0x44444444};

    /* Enlazar en circulo: system -> p1 -> p2 -> p3 -> system */
    p1.flink = &p2;
    p2.flink = &p3;
    p3.flink = &p4;
    p4.flink = &system_proc;

    system_proc.flink = &p1;

    printf("[+] antes: p3.token = 0x%llX\n", (unsigned long long) p3.token);
    steal_token(&p3, &p1);
    printf("[+] despues: p3.token = 0x%llX\n", (unsigned long long) p3.token);

    return 0;
}
