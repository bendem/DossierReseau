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


int   RequeteCoutBDEF(char*, int, int);
int   RequetePaiementBDEF(char*, int, int);
char  LocalReadChar();
int   RecoverFichierTransactionBDEF(char*);
void  HandlerSigAlarm(int);
float CalculCoutBDEF(int, int);
int   TimeToMinutesBDEF(int);


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
        printf("Usage : ./pay ip_pay port_pay ip_server port_server\n");
        exit(1);
    }

    armerSignal(SIGALRM,HandlerSigAlarm);

    sprintf(NomFichier, "LogPay-%d.dat", atoi(argv[2]));

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
        printf("1) Paiement ticket  \n");
        printf("2) Affichage du fichier \n");
        printf("3) Exit\n");
        printf("---------------------\n");
        c = LocalReadChar();
        switch(c) {
            case '1':
                res = RequeteCoutBDEF(NomFichier, GetTimeBDEF(), 0);
                if(res > 0) {
                    printf("Vous avez bien paye le ticket `%d'.\n", res);
                } else {
                    printf("Il y a eu une erreur lors du payement de votre ticket :\n\t");
                    switch(res) {
                        case 0:
                            printf("Ticket non trouve...\n");
                            break;
                        case -2:
                            printf("Ticket deja paye...\n");
                            break;
                        case -3:
                            printf("Pourquoi tu veux pas payer?? :'(\n");
                            break;
                        default:
                            printf("Erreur generique...\n");

                    }
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

int RequeteCoutBDEF(char* Fichier, int heure, int reessai) {
    struct RequeteBDEF req;
    int rc;
    float cout;
    char c;

    req.Type = Question;
    req.Action = PAIEMENT;
    req.NumTransac = NumTransac;

    req.NumeroTicket = reessai ? reessai : GetNumTicketBDEF();

    rc = SendDatagram(Desc, &req, sizeof(struct RequeteBDEF), &psoc);
    if (rc == -1) {
        perror("SendDatagram error");
    } else {
        fprintf(stderr, "Envoi de %d bytes\n", rc);
    }

    for(;;) {
        alarm(15);
        rc = ReceiveDatagram(Desc, &req, sizeof(struct RequeteBDEF), &psor);
        if (rc == -1) {
            if (errno == EINTR && IsSigAlarm == 1) {
                IsSigAlarm = 0;
                return RequeteCoutBDEF(Fichier, heure, req.NumeroTicket);
            }

            perror("ReceiveDatagram");
            return -1;
        } else {
            if(NumTransac != req.NumTransac || req.Type != Question) {
                printf("Reponse ignoree...\n");
                continue;
            }
            alarm(0);
            fprintf(stderr, "bytes:%d:%dh\n", rc, req.Heure);
            if (req.Heure > 0) {
                cout = CalculCoutBDEF(req.Heure, heure);
                printf("Confirmez-vous le paiement de %.2f ?\n", cout);
                c = LocalReadChar();
                if (c == 'O' || c == 'o') {
                    return RequetePaiementBDEF(Fichier, req.NumeroTicket, heure);
                } else {
                    return -3;
                }
            }
            // En cas d'erreur, on retourne le code en question
            return req.Heure;
        }
    }
}

int RequetePaiementBDEF(char *Fichier, int NumTicket, int Heure) {
    struct RequeteBDEF req;
    int rc;

    req.NumeroTicket = NumTicket;
    req.Type = Reponse;
    req.Action = PAIEMENT;
    req.NumTransac = NumTransac;
    req.Heure = Heure;

    rc = SendDatagram(Desc, &req, sizeof(struct RequeteBDEF), &psoc);
    if (rc == -1) {
        perror("SendDatagram error");
    } else {
        fprintf(stderr, "Envoi de %d bytes\n", rc);
    }

    for(;;) {
        alarm(15);
        rc = ReceiveDatagram(Desc, &req, sizeof(struct RequeteBDEF), &psor);
        if (rc == -1) {
            if (errno == EINTR && IsSigAlarm == 1) {
                IsSigAlarm = 0;
                return RequetePaiementBDEF(Fichier, NumTicket, Heure);
            }
            perror("ReceiveDatagram");
            return -1;
        }
        if (NumTransac != req.NumTransac || req.Type != Reponse) {
            printf("Reponse ignoree...\n");
            continue;
        }

        alarm(0);
        fprintf(stderr, "bytes:%d:%d\n", rc, req.NumeroTicket);
        if (req.NumeroTicket > 0) {
            // Ceci ne marche pas vu que la fonction cherche pour une transaction de type RESERVATION
            // et qu'il n'y en a pas dans ce fichier si...
            PaiementTicketBDEF(Fichier, GetIP(&psoo), GetPort(&psoo), NumTransac, req.Heure, req.NumeroTicket);
            ++NumTransac;
        }
        return req.NumeroTicket;
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

float CalculCoutBDEF(int heureEntree, int heurePaiement) {
    // 1 Est à changer pour changer le prix par minute...
    return 1 * (TimeToMinutesBDEF(heurePaiement) - TimeToMinutesBDEF(heureEntree));
}

int TimeToMinutesBDEF(int t) {
    int hours = t / 100;
    int minutes = t - hours * 100;
    return hours * 60 + minutes;
}
