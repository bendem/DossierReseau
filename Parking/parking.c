#include "parking.h"

void LocalIpv4ToS(int ip, char *str){
    int NM[4];
    if (ip==0)
        strcpy(str, "0.0.0.0");
    else{
        NM[0] = ( ip >> 24 ) & 255;
        NM[1] = ( ip >> 16 ) & 255;
        NM[2] = ( ip >> 8 ) & 255;
        NM[3] = ip & 255;
        sprintf(str, "%d.%d.%d.%d", NM[0], NM[1], NM[2], NM[3]);
    }
}

void MonPrintf(char* tempo, int espace, int taille ){
    int Count;
    printf("%s", tempo);
    Count = espace-taille;
    while ( Count>0 ){
        printf(" ");
        Count--;
    }
}

int CreationFichierTransaction( char *Nom , int nombreplaces ){
    FILE *Fichier;
    struct Transaction  UneTransaction;
    int nbr;

    Fichier = fopen(Nom, "w+");
    if ( Fichier == NULL ){
        fprintf(stderr, "Echec Ouverture\n");
        return(-1);
    }
    else{
        fprintf(stderr, "Ouverture reussie \n");
    }
    fseek(Fichier, 0, SEEK_END);
    /* fseek ne retourne pas la nouvelle position mais ftell */
    nbr = ftell(Fichier);
    fprintf(stderr, "Position dans le fichier %d\n", nbr);
    if (nbr>0){
        fprintf(stderr, "Fichier non vide \n");
        return(-1);
    }
    UneTransaction.IP=0 ;
    UneTransaction.Port=0;
    UneTransaction.NumTransac=0 ;
    UneTransaction.Heure=0;
    UneTransaction.PlacesLibres = nombreplaces;
    UneTransaction.NumTicket=0;
    UneTransaction.UneAction=ENTETE;
    nbr = fwrite(&UneTransaction, sizeof(struct Transaction), 1, Fichier);
    if (!nbr){
       fprintf(stderr, "Erreur Ecriture fichier \n");
       return(-1);
    }
    fclose(Fichier);
    return(1);
}

int AffichageFichier(char *Nom){
    FILE *Fichier;
    int nbr;
    struct Transaction  UneTransaction;
    char tempo[80];
    char *PS;

    Fichier = fopen(Nom, "r");
    if ( Fichier == NULL ){
        fprintf(stderr, "Echec Ouverture\n");
        return(-1);
    }
    else
        fprintf(stderr, "Ouverture reussie \n");

    PS ="IP" ; MonPrintf(PS, 20, strlen(PS));
    PS="Port"; MonPrintf(PS, 7, strlen(PS));
    PS="NumTr"; MonPrintf(PS, 7, strlen(PS));
    PS="Heure"; MonPrintf(PS, 7, strlen(PS));
    PS="Places"; MonPrintf(PS, 7, strlen(PS));
    PS="Ticket"; MonPrintf(PS, 7, strlen(PS));
    printf("\tAction\n");
    nbr = fread(&UneTransaction, sizeof(struct Transaction), 1, Fichier );

    while(!feof(Fichier)){
        char IPDD[40];

        LocalIpv4ToS(UneTransaction.IP, IPDD );  MonPrintf(IPDD, 20, strlen(IPDD));
        sprintf(tempo, "%d", UneTransaction.Port ); MonPrintf(tempo, 7, strlen(tempo));
        sprintf(tempo, "%d", UneTransaction.NumTransac ); MonPrintf(tempo, 7, strlen(tempo));
        sprintf(tempo, "%d", UneTransaction.Heure );  MonPrintf(tempo, 7, strlen(tempo));
        sprintf(tempo, "%d", UneTransaction.PlacesLibres );  MonPrintf(tempo, 7, strlen(tempo));
        sprintf(tempo, "%d", UneTransaction.NumTicket ); MonPrintf(tempo, 7, strlen(tempo));
        printf("%d\n", UneTransaction.UneAction );
        nbr = fread(&UneTransaction, sizeof(struct Transaction), 1, Fichier );
    }
    fclose(Fichier);
}

int ReservationTicketBDEF(char *Nom, int IP,int Port,int NumTransac,int Heure, int *PlacesLibres) {
    struct Transaction  UneTransaction;
    FILE *fp = fopen(Nom, "r+");
    if(fp == NULL) {
        return -1;
    }
    fread(&UneTransaction, sizeof(struct Transaction), 1, fp);
    if(UneTransaction.PlacesLibres == 0) {
        return -1;
    }

    ++UneTransaction.NumTicket;
    --UneTransaction.PlacesLibres;
    fseek(fp, 0, SEEK_SET);
    fwrite(&UneTransaction, sizeof(struct Transaction), 1, fp);
    *PlacesLibres = UneTransaction.PlacesLibres;
    UneTransaction.IP = IP;
    UneTransaction.Port = Port;
    UneTransaction.NumTransac = NumTransac;
    UneTransaction.Heure = Heure;
    UneTransaction.PlacesLibres = 0;
    UneTransaction.UneAction = RESERVATION;
    fseek(fp, 0, SEEK_END);
    fwrite(&UneTransaction, sizeof(struct Transaction), 1, fp);
    fclose(fp);
    return 0;
}

int PaiementTicketBDEF(char *Nom, int IP, int Port, int NumTransac, int Heure, int NumTicket) {
    struct Transaction  UneTransaction;
    FILE *fp = fopen(Nom, "r+");
    if(fp == NULL) {
        return -1;
    }
    int ticketFound = -1;

    // On passe l'entête...
    fseek(fp, sizeof(Transaction), SEEK_SET);
    while (fread(&UneTransaction, sizeof(struct Transaction), 1, fp)) {
        if(UneTransaction.NumTicket == NumTicket) {
            if(UneTransaction.Action == RESERVATION) {
                ticketFound = ftell(fp);
            }
            // Si le ticket est déjà payé ou que la voiture est déjà sortie...
            if(UneTransaction.Action == PAIEMENT || UneTransaction.Action == SORTIE) {
                fclose(fp);
                return -2;
            }
        }
    }

    if(ticketFound == -1) {
        return -3;
        fclose(fp);
    }

    // Paiement
    UneTransaction.IP = IP;
    UneTransaction.Port = Port;
    UneTransaction.NumTransac = NumTransac;
    UneTransaction.Heure = Heure;
    UneTransaction.PlacesLibres = 0;
    UneTransaction.UneAction = PAIEMENT;
    fseek(fp, 0, SEEK_END);
    fwrite(&UneTransaction, sizeof(struct Transaction), 1, fp);
    fclose(fp);

    return 0;
}
