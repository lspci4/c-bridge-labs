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
        if (p->pid == 4) {
            current->token = p->token;
            printf("[+] token robado\n");
            return;
        }
        p = p->flink;
    } while (p != head);
}

int main(void) {
    Process system_proc = {.pid = 4, .token = 0xDEADBEEF};
    Process p1 = {.pid = 100, .token = 0x11111111};
    Process p2 = {.pid = 200, .token = 0x22222222};
    Process p3 = {.pid = 300, .token = 0x33333333};

    system_proc.flink = &p1;
    p1.flink = &p2;
    p2.flink = &p3;
    p3.flink = &system_proc;

    printf("[+] antes: p3.token = 0x%llX\n", (unsigned long long) p3.token);
    steal_token(&p3, &system_proc);
    printf("[+] despues: p3.token = 0x%llX\n", (unsigned long long) p3.token);

    return 0;
}
