/*--------------------------------------
Herman Vanstapel

ex02\ser.c

Un serveur recevant une structure et lié à un client particulier
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../physlib/physlib.h"
#include "../Parking/parking.h"
#include "structure.h"

int main(int argc, char *argv[]) {
    int rc;
    int Desc;
    struct sockaddr_in psoo; /* o = origine */
    struct sockaddr_in psoc; /* s = cible */
    struct sockaddr_in psor; /* r = remote */

    struct RequeteBDEF notreRequetePerso;

    memset(&psoo, 0, sizeof(struct sockaddr_in));
    memset(&psoc, 0, sizeof(struct sockaddr_in));
    memset(&psor, 0, sizeof(struct sockaddr_in));

    printf("Ceci est le serveur\n");
    if (argc!=5)
    {
        printf("ser ser port cli port\n");
        exit(1);
    }

    Desc = CreateSockets(&psoo, &psoc, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
    if (Desc == -1) {
        perror("CreateSockets");
    } else {
        printf(" CreateSockets : %d \n", Desc);
    }

    rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
    if (rc == -1) {
        perror("ReceiveDatagram");
    } else {
        fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.Action);
    }

    /* reponse avec psoc */
    notreRequetePerso.Type = Reponse;
    notreRequetePerso.NumeroTicket = 2;
    rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psoc);
    if (rc == -1) {
        perror("SendDatagram:");
    } else {
        fprintf(stderr, "bytes:%d\n", rc);
    }

    /* reponse avex psor r = remote */

    // strcat(notreRequetePerso.Message, "X2");
    // rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
    // if (rc == -1) {
    //     perror("SendDatagram:");
    // } else {
    //     fprintf(stderr, "bytes:%d\n", rc);
    // }

    return 0;
}
