#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define KEY   1234

typedef struct {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(void)
{
    int msgid;
    message_buf sbuf, rbuf;
    size_t len;

    if ((msgid = msgget(KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    len = strlen(sbuf.mtext) + 1;

    if (msgsnd(msgid, &sbuf, len, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("P1 -> P2 [tipo 1]: %s\n", sbuf.mtext);

    if (msgrcv(msgid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("P1 <- P2 [tipo 2]: %s\n", rbuf.mtext);

    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "I can hear you too");
    len = strlen(sbuf.mtext) + 1;

    if (msgsnd(msgid, &sbuf, len, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("P1 -> P2 [tipo 3]: %s\n", sbuf.mtext);

    return 0;
}