/*---------------------------------------------------------------
Vanstapel Herman
ex02\cli.c

Le client dit bonjour en utilisant un structure et
le serveur fait de même
------------------------------------------------------------------*/
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

    if (argc != 5) {
        printf("cli client portc serveur ports\n");
        exit(1);
    }
    Desc = CreateSockets(&psoo, &psoc, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
    if (Desc == -1) {
        perror("CreateSockets:");
    } else {
        fprintf(stderr, "CreateSockets %d\n", Desc);
    }

    notreRequetePerso.Type = Question;
    notreRequetePerso.Action = RESERVATION;
    notreRequetePerso.NumTransac = 0;

    rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psoc);

    if (rc == -1) {
        perror("SendDatagram error");
    } else {
        fprintf(stderr, "Envoi de %d bytes\n", rc);
    }

    // memset(&notreRequetePerso, 0, sizeof(struct RequeteBDEF));
    // tm = sizeof(struct RequeteBDEF);

    rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
    if (rc == -1) {
        perror("ReceiveDatagram");
    } else {
        fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.NumeroTicket);
    }

	notreRequetePerso.Type = Question;
    notreRequetePerso.Action = RESERVATION;
    notreRequetePerso.NumTransac = 0;


	rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psoc);

    if (rc == -1) {
        perror("SendDatagram error");
    } else {
        fprintf(stderr, "Envoi de %d bytes\n", rc);
    }

    // memset(&notreRequetePerso, 0, sizeof(struct RequeteBDEF));
    // tm = sizeof(struct RequeteBDEF);

    rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
    if (rc == -1) {
        perror("ReceiveDatagram");
    } else {
        fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.NumeroTicket);
    }
    // memset(&notreRequetePerso, 0, sizeof(struct RequeteBDEF));
    // tm = sizeof(struct RequeteBDEF);

    // rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
    // if (rc == -1) {
    //     perror("ReceiveDatagram");
    // } else {
    //     fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.NumeroTicket);
    // }

    close(Desc);
}
