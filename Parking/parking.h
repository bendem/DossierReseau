#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DelNewLine(char *Chaine);
char ReadChar();

enum Action { ENTETE = 1, RESERVATION = 2, PAIEMENT = 3, SORTIE = 4 };

struct Transaction {
    int IP;
    int Port;
    int NumTransac;
    int	Heure;
    int PlacesLibres;
    int NumTicket;
    enum Action UneAction;
};

int CreationFichierTransaction( char *Nom, int nombreplaces);
int ReservationTicketBDEF(char *Nom, int IP,int Port,int NumTransac,int Heure, int *PlacesLibres);
int AffichageFichier(char *Nom);
int RechercheHeureTicket(char *Nom,enum Action UneAction, int NumTicket);
int PaiementTicketBDEF(char *Nom, int IP,int Port,int NumTransac,int Heure,int NumTicket);
int SortieParking(char *Nom, int IP,int Port,int NumTransac,int Heure,int NumTicket);
int RechercheTicketTransaction(char *Nom,int IP,int Port,int NumTransac,enum Action UneAction);
