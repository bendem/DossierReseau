/*---------------------------------------------------------------
   Vanstapel Herman
   ex01\cli.c

 ex01 Le client dit bonjour et
      le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../physlib/physlib.h"

int main(int argc, char *argv[])
{
 int rc;
 int Desc;
 char *message;
 char reponse[100 ];
 int tm;

 struct sockaddr_in psoo; /* o = origine */
 struct sockaddr_in psos; /* s = serveur */
 struct sockaddr_in psor; /* r = remote */

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
 message = "Bonjour ";

 rc = SendDatagram(Desc, message, strlen(message)+1, &psos);

 if (rc == -1)
    perror("SendDatagram");
 else
   fprintf(stderr, "Envoi de %d bytes\n", rc);

  memset(reponse, 0, sizeof(reponse));
 tm = sizeof(reponse);
  rc = ReceiveDatagram(Desc, reponse, tm, &psor);
 if (rc == -1)
    perror("ReceiveDatagram");
 else
   fprintf(stderr, "bytes:%d:%s\n", rc, reponse);

 memset(reponse, 0, sizeof(reponse));
 tm = sizeof(reponse);
  rc = ReceiveDatagram(Desc, reponse, tm, &psor);
 if (rc == -1)
    perror("ReceiveDatagram");
 else
   fprintf(stderr, "bytes:%d:%s\n", rc, reponse);
 close(Desc);
}
