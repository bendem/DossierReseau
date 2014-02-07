#include "../Parking/parking.h"
#define NOMDEFICHIER "transaction"

char LocalReadChar() {
    char Tampon[80];
    fgets(Tampon, sizeof Tampon, stdin);
    return Tampon[0];
}

int ctoi(char d) {
    char str[2];

    str[0] = d;
    str[1] = '\0';
    return (int) strtol(str, NULL, 10);
}

int main(void) {
    char c, b;
    int res;
    int ip;
    int Heure, HeureS, HeureP;
    int PlacesLibres;
    char Tampon[50];
    int NumTicket;

    while(1) {
        printf ("menu :\n");
        printf("1) Creation du fichier  \n");
        printf("2) Affichage du fichier \n");
        printf("3) Reservation Ticket \n");
        printf("4) Recherche et paiement \n");
        printf("5) Recherche et sortie \n");
        printf("6) exit\n");
        printf("---------------------\n");
        c = LocalReadChar();   /* Readchar se trouve dans la librairie Physlib */
        printf("\n%c\n", c);
        switch(c) {
            case '1' :
                res = CreationFichierTransaction(NOMDEFICHIER, 50);
                printf("Resultat %d \n", res);
                break;
            case '2' :
                res = AffichageFichier(NOMDEFICHIER);
                printf("Resultat %d \n", res);
                break;
            case '3':
                Ipv4ToInt("192.168.1.1", &ip);
                res = ReservationTicketBDEF(NOMDEFICHIER, ip, 80, 3, 16, &PlacesLibres);
                printf("Resultat %d \n", res);
                break;
            case '4':
                printf("Num Ticket : ");
                b = LocalReadChar();
                Ipv4ToInt("192.168.1.2", &ip);
                res = PaiementTicketBDEF(NOMDEFICHIER, ip, 80, 3, 1812, ctoi(b));
                printf("Resultat %d \n", res);
                break;
            case '5':
                printf("Num Ticket : ");
                b = LocalReadChar();
                Ipv4ToInt("192.168.2.2", &ip);
                res = SortieParkingBDEF(NOMDEFICHIER, ip, 80, 3, 1812, ctoi(b));
                printf("Resultat %d \n", res);
                break;
            case '6' :
                exit(0);
        }
    }
    return 0;
}
