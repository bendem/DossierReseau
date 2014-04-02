/*--------------------------------------
Herman Vanstapel

ex02\ser.c

Un serveur recevant une structure et lié à un client particulier
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "../physlib/physlib.h"
#include "../Parking/parking.h"
#include "structure.h"
#include "crc.h"

#define NOMDEFICHIER "transaction.dat"

int processDatagramBDEF(struct RequeteBDEF, struct sockaddr_in);


int main(int argc, char *argv[]) {
    int rc;
    char c;
    int Desc;
    struct sockaddr_in psoo; /* o = origine */
    struct sockaddr_in psor; /* r = remote */

    struct RequeteBDEF notreRequetePerso;

    printf("Ceci est le serveur\n");
    if (argc != 3) {
        printf("Usage de la commande : ./ser ip_server port_server\n");
        exit(1);
    }

    Desc = CreateSockets(&psoo, NULL, argv[1], atoi(argv[2]), NULL, 0);
    if (Desc == -1) {
        perror("CreateSockets");
    } else {
        printf("CreateSockets : %d \n", Desc);
    }

    for (;;) {
        printf("---> Pause, appuyer sur <enter> pour continuer <---\n");
        getchar();
        rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
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

        /* reponse avec psoc */
        notreRequetePerso.CRC = 0;
    	notreRequetePerso.CRC = cksum(&notreRequetePerso, sizeof(notreRequetePerso));
    	printf("Voulez-vous changer le CRC avant l'envoi? : \n");
    	c = getchar();
	    if (c != '\n')
	    {
	        notreRequetePerso.CRC++;
	    }
    	printf("CRC: %u, cksum: %d\n", notreRequetePerso.CRC, cksum(&notreRequetePerso, sizeof(notreRequetePerso)));
        rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
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
