/*---------------------------------------------------------------
   Vanstapel Herman
   cphex\cli.c

 Le client dit bonjour et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>  /* ces deux include pour le getpid */
#include <string.h>
#include "../physlib/physlib.h"
#include "structure.h"

sigjmp_buf env;
int mask;
int NbrAlarmes=0;

static void signal_handler(int sig)
{
   NbrAlarmes++;
   siglongjmp(env, sig);
}

static void signal_alarm(int sig)
{
   NbrAlarmes++;
   fprintf(stderr, "Alarme !!!!!!!!\n");
}



int main(int argc, char *argv[])
{
 int rc;
 int Desc;
 int tm;
 int Port, Ip;
 char Tampon[80];
 int returned_from_longjump;
 unsigned int time_interval;
 int ret;
 int Compteur = 0;

 struct sockaddr_in psoo; /* o = origine */
 struct sockaddr_in psos; /* s = serveur */
 struct sockaddr_in psor; /* r = remote */
 struct Requete UneRequete;
 char *pret;

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
    fprintf(stderr, "CreateSockets %d !!!!\n", Desc);

  Ip = GetIP(&psoo);
  Port = GetPort(&psoo);
  memset(Tampon, 0, sizeof(Tampon));
  Ipv4ToS(Ip, Tampon);
  printf("Adresse Ip du client %s : Port %d \n", Tampon, Port);

 Compteur = 0;
 printf(" GetPid %d \n", getpid());
 while(1)
 {
   /* ici setjmp */
   returned_from_longjump = sigsetjmp(env, 1); /* <> 0 pour sauver le masque, ne surtout pas utiliser setjmp */
   if (returned_from_longjump!= 0)
     switch (returned_from_longjump)	 {
	case SIGINT:
	     printf("longjumped from interrupt %d\n", SIGINT);
	     exit(0);
	     break;
	case SIGALRM:
	     printf("longjumped from alarm %d\n", SIGALRM);
	     printf("Alarme numero %d \n", NbrAlarmes);
	     if (NbrAlarmes==4)
	         exit(0);
	     break;
     };

  signal(SIGINT, signal_handler);
  signal(SIGUSR1, signal_handler);
  signal(SIGALRM, signal_alarm);

   UneRequete.Compteur = Compteur;
   UneRequete.Type = Question;
   strncpy(UneRequete.Message, "Hello Multiclient", sizeof(UneRequete.Message));
   rc = SendDatagram(Desc, &UneRequete, sizeof(struct Requete), &psos);
   fprintf(stderr, "Turlututu \n");
   if (rc == -1)
      perror("SendDatagram");
   else
      fprintf(stderr, "Envoi du message %d avec %d bytes\n", UneRequete.Compteur, rc);

   memset(&UneRequete, 0, sizeof(struct Requete));
   tm = sizeof(struct Requete);

   time_interval = 5;
   ret = alarm(time_interval);
   // signal(SIGALRM, signal_alarm);
   tm = sizeof(UneRequete);
   printf("Ecoute sur le descripeur %d \n", Desc);
   rc = ReceiveDatagram(Desc, &UneRequete, tm, &psor);
   fprintf(stderr, "raison:%d \n", rc);
   pret = strerror(errno);
   fprintf(stderr, "Message est : %s \n", pret);
   ret = alarm(0);
   if (rc == -1)
   {
       perror("ReceiveDatagram");
   }
   else
   {
      fprintf(stderr, "bytes:%d:%s\n", rc, UneRequete.Message);
   }
   Compteur++;
   sleep(5);
 }

 close(Desc);
}
