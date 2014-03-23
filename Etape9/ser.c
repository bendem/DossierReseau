/*--------------------------------------
Herman Vanstapel

ex02\ser.c

Un serveur recevant une structure et lié à un client particulier
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "../physlib/physlib.h"
#include "../Parking/parking.h"
#include "structure.h"
#include "crc.h"

#define NOMDEFICHIER "transaction.dat"

int processDatagramBDEF(struct RequeteBDEF, struct sockaddr_in);

int main(int argc, char *argv[]) {
    int rc;
    int DescE, DescP, DescS, currentDesc;
    fd_set readfs;
    struct sockaddr_in psor; /* remote */
    struct sockaddr_in psoE; /* Entree */
    struct sockaddr_in psoP; /* Paiement */
    struct sockaddr_in psoS; /* Sortie */

    struct RequeteBDEF notreRequetePerso;

    printf("Ceci est le serveur\n");
    if (argc != 5) {
        printf("Usage de la commande : ./ser <ip server> <port entree> <port paiement> <port sortie>\n");
        exit(1);
    }

    DescE = CreateSockets(&psoE, NULL, argv[1], atoi(argv[2]), NULL, 0);
    if (DescE == -1) {
        perror("CreateSockets psoE");
    } else {
        printf("CreateSockets : %d \n", DescE);
    }
    DescP = CreateSockets(&psoP, NULL, argv[1], atoi(argv[3]), NULL, 0);
    if (DescP == -1) {
        perror("CreateSockets psoP");
    } else {
        printf("CreateSockets : %d \n", DescP);
    }
    DescS = CreateSockets(&psoS, NULL, argv[1], atoi(argv[4]), NULL, 0);
    if (DescS == -1) {
        perror("CreateSockets psoS");
    } else {
        printf("CreateSockets : %d \n", DescS);
    }

    for (;;) {
        // On veux écouter sur pls ports
        FD_ZERO(&readfs);
        FD_SET(DescE, &readfs);
        FD_SET(DescP, &readfs);
        FD_SET(DescS, &readfs);

        // printf("---> Pause, appuyer sur <enter> pour continuer <---\n");
        // getchar();
        if((rc = select(FD_SETSIZE, &readfs, NULL, NULL, NULL)) < 0) {
            perror("select error");
            exit(errno);
        }

        // On lit le datagramme sur le port reçu
        if(FD_ISSET(DescE, &readfs)) {
            currentDesc = DescE;
        } else if(FD_ISSET(DescP, &readfs)) {
            currentDesc = DescP;
        } else if(FD_ISSET(DescS, &readfs)) {
            currentDesc = DescS;
        }
        rc = ReceiveDatagram(currentDesc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);

        if (rc == -1) {
            perror("ReceiveDatagram");
        } else {
            fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.Action);
        }
        printf("CRC: %u, cksum: %u\n", notreRequetePerso.CRC, cksum(&notreRequetePerso, sizeof(notreRequetePerso)));
        if(cksum(&notreRequetePerso, sizeof(notreRequetePerso)) != 0 && notreRequetePerso.Action == RESERVATION) {
            printf("Mauvais checksum! On l'ignore...\n");
            continue;
        }

        if(notreRequetePerso.Action == PAIEMENT && notreRequetePerso.Type == Question) {
            notreRequetePerso.Heure = processDatagramBDEF(notreRequetePerso, psor);
        } else {
            notreRequetePerso.NumeroTicket = processDatagramBDEF(notreRequetePerso, psor);
        }
        notreRequetePerso.Type = Reponse;

        /* reponse avec psor */
        rc = SendDatagram(currentDesc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
        if (rc == -1) {
            perror("SendDatagram:");
        } else {
            fprintf(stderr, "bytes:%d\n", rc);
        }
    }

    return 0;
}

int processDatagramBDEF(struct RequeteBDEF data, struct sockaddr_in psor) {
    switch(data.Action) {
        case ENTETE:
            return -15;
        case RESERVATION:
            return ReservationTicketBDEF(
                NOMDEFICHIER,
                ntohl(psor.sin_addr.s_addr),
                ntohs(psor.sin_port),
                data.NumTransac,
                data.Heure,
                NULL
            );
        case PAIEMENT:
            if(data.Type == Question) {
                return GetTicketTimeBDEF(NOMDEFICHIER, data.NumeroTicket, RESERVATION);
            }
            return PaiementTicketBDEF(
                NOMDEFICHIER,
                ntohl(psor.sin_addr.s_addr),
                ntohs(psor.sin_port),
                data.NumTransac,
                data.Heure,
                data.NumeroTicket
            );
        case SORTIE:
            return SortieParkingBDEF(
                NOMDEFICHIER,
                ntohl(psor.sin_addr.s_addr),
                ntohs(psor.sin_port),
                data.NumTransac,
                data.Heure,
                data.NumeroTicket
            );
    }
}
