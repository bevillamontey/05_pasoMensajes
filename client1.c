#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ          128
#define KEY            1234
#define MSG_REQ        1
#define MSG_RESP_BASE  100
#define CLIENT_ID      1

typedef struct {
    long mtype;
    int  client_id;
    char mtext[MSGSZ];
} message_buf;

#define MSG_SIZE (sizeof(message_buf) - sizeof(long))

int main(void)
{
    int msgid;
    message_buf req, resp;

    if ((msgid = msgget(KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    req.mtype = MSG_REQ;
    req.client_id = CLIENT_ID;
    snprintf(req.mtext, MSGSZ, "Hola servidor, soy el cliente %d", CLIENT_ID);

    if (msgsnd(msgid, &req, MSG_SIZE, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("Cliente %d envio peticion: %s\n", CLIENT_ID, req.mtext);

    if (msgrcv(msgid, &resp, MSG_SIZE,
               MSG_RESP_BASE + CLIENT_ID, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Cliente %d recibio: %s\n", CLIENT_ID, resp.mtext);

    return 0;
}
