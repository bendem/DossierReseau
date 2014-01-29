#include "../Parking/parking.h"

char LocalReadChar() {
    char Tampon[80];
    fgets(Tampon, sizeof Tampon, stdin);
    return Tampon[0];
}

int main(void) {
    char c;
    int res;
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
            case '6' :
            exit(1);
        }
    }
    return(1);
}
