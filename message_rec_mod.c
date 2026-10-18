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
    message_buf rbuf, sbuf;
    size_t len;

    if ((msgid = msgget(KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (msgrcv(msgid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("P2 <- P1 [tipo 1]: %s\n", rbuf.mtext);

    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    len = strlen(sbuf.mtext) + 1;

    if (msgsnd(msgid, &sbuf, len, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("P2 -> P1 [tipo 2]: %s\n", sbuf.mtext);

    if (msgrcv(msgid, &rbuf, MSGSZ, 3, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("P2 <- P1 [tipo 3]: %s\n", rbuf.mtext);

    if (msgctl(msgid, IPC_RMID, NULL) < 0) {
        perror("msgctl");
        exit(1);
    }
    printf("P2: cola eliminada.\n");

    return 0;
}