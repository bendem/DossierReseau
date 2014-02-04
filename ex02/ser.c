/*--------------------------------------
  Herman Vanstapel

  ex02\ser.c

  Un serveur recevant une structure et lié à un client particulier
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
 if (argc!=5)
 {
  printf("ser ser port cli port\n");
  exit(1);
 }

 Desc = CreateSockets(&psoo, &psos, argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
 if (Desc == -1)
    perror("CreateSockets");
 else
    printf(" CreateSockets : %d \n", Desc);

 tm = sizeof(struct Requete);
 rc = ReceiveDatagram(Desc, &UneRequete, tm, &psor);
 if (rc == -1)
    perror("ReceiveDatagram");
 else
   fprintf(stderr, "bytes:%d:%s\n", rc, UneRequete.Message);

 /* reponse avec psos */

 UneRequete.Type = Reponse;
 strcat(UneRequete.Message, " Client");
 rc = SendDatagram(Desc, &UneRequete, sizeof(struct Requete), &psos);
 if (rc == -1)
    perror("SendDatagram:");
 else
   fprintf(stderr, "bytes:%d\n", rc);

 /* reponse avex psor r = remote */

 strcat(UneRequete.Message, "X2");
 rc = SendDatagram(Desc, &UneRequete, sizeof(struct Requete), &psor);
 if (rc == -1)
    perror("SendDatagram:");
 else
   fprintf(stderr, "bytes:%d\n", rc);

}
