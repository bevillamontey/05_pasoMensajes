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
    int i;

    if ((msgid = msgget(KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    printf("Servidor listo. Esperando peticiones en mtype=%d...\n", MSG_REQ);

    for (i = 0; i < 2; i++) {
        if (msgrcv(msgid, &req, MSG_SIZE, MSG_REQ, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Servidor recibio de cliente %d: %s\n",
               req.client_id, req.mtext);

        resp.mtype = MSG_RESP_BASE + req.client_id;
        resp.client_id = req.client_id;

        snprintf(resp.mtext, MSGSZ,
                 "Respuesta privada para cliente %d: recibi '%s'",
                 req.client_id, req.mtext);

        if (msgsnd(msgid, &resp, MSG_SIZE, 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        printf("Servidor respondio a cliente %d con mtype=%ld\n",
               req.client_id, resp.mtype);
    }

    printf("Servidor: atendidas 2 peticiones. Cola conservada.\n");
    printf("Limpie con: ipcrm -Q %d\n", KEY);

    return 0;
}
