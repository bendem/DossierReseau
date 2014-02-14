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
#include "../siglib/sigs.h"


int  RequeteReservationBDEF(char*, int);
char LocalReadChar();
int  RecoverFichierTransactionBDEF(char*);
void HandlerSigAlarm(int);

int Desc;
int IsSigAlarm = 0;
int NumTransac = 0;
struct sockaddr_in psoo; /* o = origine */
struct sockaddr_in psoc; /* s = cible */
struct sockaddr_in psor; /* r = remote */

int main(int argc, char *argv[]) {
    int res;
    char NomFichier[20];
    char c;

    armerSignal(SIGALRM,HandlerSigAlarm);

    sprintf(NomFichier, "LogClient-%d.dat", atoi(argv[2]));

    NumTransac = RecoverFichierTransactionBDEF(NomFichier);
    printf("%d\n", NumTransac);

    if (NumTransac == 0) {
        CreationFichierTransaction(NomFichier, 50);
    }

    memset(&psoo, 0, sizeof(struct sockaddr_in));
    memset(&psoc, 0, sizeof(struct sockaddr_in));
    memset(&psor, 0, sizeof(struct sockaddr_in));

    if (argc != 5) {
        printf("Usage : ./cli ip_client port_port ip_server port_server\n");
        exit(1);
    }
    Desc = CreateSockets(&psoo, &psoc, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
    if (Desc == -1) {
        perror("CreateSockets:");
    } else {
        fprintf(stderr, "CreateSockets %d\n", Desc);
    }

    for(;;) {
        printf ("\n\nMenu :\n");
        printf("1) Demande ticket  \n");
        printf("2) Affichage du fichier \n");
        printf("3) Exit\n");
        printf("---------------------\n");
        c = LocalReadChar();
        // printf("\n%c\n", c);
        switch(c) {
            case '1':
                res = RequeteReservationBDEF(NomFichier, GetTimeBDEF());
                if(res > 0) {
                    printf("Bienvenue, votre ticket porte le numero %d.\n", res);
                } else {
                    printf("Il y a eu une erreur lors de la reservation de votre ticket...\n");
                }
                break;
            case '2':
                AffichageFichier(NomFichier);
                break;
            case '3':
                close(Desc);
                return 0;
        }
    }

}

int RequeteReservationBDEF(char* Fichier, int heure){
    struct RequeteBDEF notreRequetePerso;
    int rc;

    notreRequetePerso.Type = Question;
    notreRequetePerso.Action = RESERVATION;
    notreRequetePerso.NumTransac = NumTransac;
    notreRequetePerso.Heure = heure;

    rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psoc);

    if (rc == -1) {
        perror("SendDatagram error");
    } else {
        fprintf(stderr, "Envoi de %d bytes\n", rc);
    }

    alarm(30);

    rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
    if (rc == -1) {
        if (errno == EINTR && IsSigAlarm == 1) {
        	IsSigAlarm = 0;
        	return RequeteReservationBDEF(Fichier, heure);
        }

        perror("ReceiveDatagram");
        return -1;
    } else {
        fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.NumeroTicket);
        if (notreRequetePerso.NumeroTicket > 0) {
            ReservationTicketBDEF(Fichier, GetIP(&psoo), GetPort(&psoo), NumTransac, notreRequetePerso.Heure, NULL);
            NumTransac++;
        }

        return notreRequetePerso.NumeroTicket;
    }
}

char LocalReadChar() {
    char Tampon[80];
    fgets(Tampon, sizeof Tampon, stdin);
    return Tampon[0];
}

int RecoverFichierTransactionBDEF(char* NomFichier){
    FILE* fp;
    struct Transaction Transac;

    fp = fopen(NomFichier, "r");

    if(fp == NULL){
        return 0;
    }

    fseek(fp, -sizeof(struct Transaction), SEEK_END);
    fread(&Transac, sizeof(struct Transaction), 1, fp);
    return Transac.NumTransac + 1;
}

void HandlerSigAlarm(int sig){
	IsSigAlarm = 1;
}
