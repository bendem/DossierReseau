/*---------------------------------------------------------------
   Vanstapel Herman
   ex04\cli.c

 Le client dit bonjour et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../physlib/physlib.h"
#include "structure.h"

int main(int argc, char *argv[])
{
 int rc;
 int Desc;
 int tm;
 int Port, Ip;
 char Tampon[80];

 struct sockaddr_in psoo; /* o = origine */
 struct sockaddr_in psos; /* s = serveur */
 struct sockaddr_in psor; /* r = remote */
 struct Requete UneRequete;

 memset(&psoo, 0, sizeof(struct sockaddr_in));
 memset(&psos, 0, sizeof(struct sockaddr_in));
 memset(&psor, 0, sizeof(struct sockaddr_in));

 if (argc!=5)

{
    printf("cli client portc serveur ports\n");
    exit(1);
 }
 Desc = CreateSockets(&psoo, &psos, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
 if (Desc == -1)
    perror("CreateSockets:");
 else
    fprintf(stderr, "CreateSockets %d\n", Desc);

  Ip = GetIP(&psoo);
  Port = GetPort(&psoo);
  memset(Tampon, 0, sizeof(Tampon));
  Ipv4ToS(Ip, Tampon);
  printf("Adresse Ip du client %s : Port %d \n", Tampon, Port);

 UneRequete.Type = Question;
 strncpy(UneRequete.Message, "Multiclient; Bonjour", sizeof(UneRequete.Message));

 rc = SendDatagram(Desc, &UneRequete, sizeof(struct Requete), &psos);

 if (rc == -1)
    perror("SendDatagram");
 else
   fprintf(stderr, "Envoi de %d bytes\n", rc);

 memset(&UneRequete, 0, sizeof(struct Requete));
 tm = sizeof(struct Requete);

  rc = ReceiveDatagram(Desc, &UneRequete, tm, &psor);
 if (rc == -1)
    perror("ReceiveDatagram");
 else
   fprintf(stderr, "bytes:%d:%s\n", rc, UneRequete.Message);


 close(Desc);
}
