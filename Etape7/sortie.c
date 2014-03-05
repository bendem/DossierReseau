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


int  RequeteSortieBDEF(char*, int);
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

    if (argc != 5) {
        printf("Usage : ./sortie ip_client port_port ip_server port_server\n");
        exit(1);
    }

    armerSignal(SIGALRM,HandlerSigAlarm);

    sprintf(NomFichier, "LogSortie-%d.dat", atoi(argv[2]));

    NumTransac = RecoverFichierTransactionBDEF(NomFichier);
    printf("%d\n", NumTransac);

    if (NumTransac == 0) {
        CreationFichierTransaction(NomFichier, 50);
    }

    Desc = CreateSockets(&psoo, &psoc, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
    if (Desc == -1) {
        perror("CreateSockets:");
    } else {
        fprintf(stderr, "CreateSockets %d\n", Desc);
    }

    for(;;) {
        printf ("\n\nMenu :\n");
        printf("1) Sortie du parking  \n");
        printf("2) Affichage du fichier \n");
        printf("3) Exit\n");
        printf("---------------------\n");
        c = LocalReadChar();
        // printf("\n%c\n", c);
        switch(c) {
            case '1':
                res = RequeteSortieBDEF(NomFichier, GetTimeBDEF());
                if(res > 0) {
                    printf("Cya, votre ticket portait le numero %d.\n", res);
                } else {
                    printf("Il y a eu une erreur lors de la sortie de votre ticket...\n");
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

int RequeteSortieBDEF(char* Fichier, int heure) {
    struct RequeteBDEF notreRequetePerso;
    int rc;

    notreRequetePerso.Type = Question;
    notreRequetePerso.Action = SORTIE;
    notreRequetePerso.NumTransac = NumTransac;
    notreRequetePerso.Heure = heure;
    notreRequetePerso.NumeroTicket = GetNumTicketBDEF();

    rc = SendDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psoc);

    if (rc == -1) {
        perror("SendDatagram error");
    } else {
        fprintf(stderr, "Envoi de %d bytes\n", rc);
    }

    alarm(10);

    for(;;) {
        rc = ReceiveDatagram(Desc, &notreRequetePerso, sizeof(struct RequeteBDEF), &psor);
        if (rc == -1) {
            if (errno == EINTR && IsSigAlarm == 1) {
                IsSigAlarm = 0;
                return RequeteSortieBDEF(Fichier, heure);
            }

            perror("ReceiveDatagram");
            return -1;
        } else {
            if (NumTransac!=notreRequetePerso.NumTransac) {
                continue;
            }
            fprintf(stderr, "bytes:%d:%d\n", rc, notreRequetePerso.NumeroTicket);
            if (notreRequetePerso.NumeroTicket > 0) {
                SortieParkingBDEF(Fichier, GetIP(&psoo), GetPort(&psoo), NumTransac, notreRequetePerso.Heure, notreRequetePerso.NumeroTicket);
                NumTransac++;
            }
            alarm(0);

            return notreRequetePerso.NumeroTicket;
        }
    }
}


char LocalReadChar() {
    char Tampon[80];
    fgets(Tampon, sizeof Tampon, stdin);
    return Tampon[0];
}

int RecoverFichierTransactionBDEF(char* NomFichier) {
    FILE* fp;
    struct Transaction Transac;

    fp = fopen(NomFichier, "r");

    if(fp == NULL) {
        return 0;
    }

    fseek(fp, -sizeof(struct Transaction), SEEK_END);
    fread(&Transac, sizeof(struct Transaction), 1, fp);
    return Transac.NumTransac + 1;
}

void HandlerSigAlarm(int sig) {
    IsSigAlarm = 1;
}
