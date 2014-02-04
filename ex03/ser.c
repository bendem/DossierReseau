/*--------------------------------------
  Herman Vanstapel

  ex03\ser.c
  Un serveur avec plusieurs clients
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../physlib/physlib.h"
#include "structure.h"

int main(int argc, char *argv[])
{
 int rc;
 int Desc;
 struct sockaddr_in psoo; /* o = origine */
 struct sockaddr_in psos; /* s = serveur */
 struct sockaddr_in psor; /* r = remote */

 int tm;
 struct Requete UneRequete;

 memset(&psoo, 0, sizeof(struct sockaddr_in));
 memset(&psos, 0, sizeof(struct sockaddr_in));
 memset(&psor, 0, sizeof(struct sockaddr_in));

 printf("Ceci est le serveur\n");
 if (argc!=3)
 {
  printf("ser ser port \n");
  exit(1);
 }

 Desc = CreateSockets(&psoo, NULL, argv[1], atoi(argv[2]), NULL, 0);
 if (Desc == -1)
    perror("CreateSockets");
 else
    printf(" CreateSockets : %d \n", Desc);

 while(1)
 {
  tm = sizeof(struct Requete);
  rc = ReceiveDatagram(Desc, &UneRequete, tm, &psor);
  if (rc == -1)
    perror("ReceiveDatagram");
  else
   fprintf(stderr, "bytes:%d:%s\n", rc, UneRequete.Message);

  /* reponse avec psor qui contient toujours l'adresse du dernier client */

  UneRequete.Type = Reponse;
  strcat(UneRequete.Message, " Client");
  rc = SendDatagram(Desc, &UneRequete, sizeof(struct Requete), &psor);
  if (rc == -1)
    perror("SendDatagram:");
  else
    fprintf(stderr, "bytes:%d\n", rc);
 }
}
