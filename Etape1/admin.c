#include "../Parking/parking.h"

char LocalReadChar() {
    char Tampon[80];
    fgets(Tampon, sizeof Tampon, stdin);
    return Tampon[0];
}

int main(void) {
    char c;
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
                res = CreationFichierTransaction("essai", 50);
                printf("Resultat %d \n", res);
                break;
            case '2' :
                res = AffichageFichier("essai");
                printf("Resultat %d \n", res);
                break;
            case '3':
                Ipv4ToInt("192.168.1.1", &ip);
                res = ReservationTicketBDEF("essai", ip, 80, 3, 16, &PlacesLibres);
                printf("Resultat %d \n", res);
                break;
            case '4':
                Ipv4ToInt("192.168.1.2", &ip);
                res = PaiementTicketBDEF("essai", ip, 80, 3, 1812, 2);
                printf("Resultat %d \n", res);
                break;
            case '5':
                break;
            case '6' :
                exit(0);
        }
    }
    return 0;
}
