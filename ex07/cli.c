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
int mask ; 
int NbrAlarmes=0 ;

static void signal_handler(int sig)
{
   
   siglongjmp(env,sig);
}

int main(int argc, char *argv[])
{
 int rc ;
 int Desc ;
 int tm ; 
 int Port, Ip ;
 char Tampon[80] ;
 int returned_from_longjump ;
 unsigned int time_interval  ; 
 int ret ;
 int Compteur = 0 ;
 
 struct sockaddr_in psoo ; /* o = origine */
 struct sockaddr_in psos ; /* s = serveur */
 struct sockaddr_in psor ; /* r = remote */
 struct Requete UneRequeteE ;
 struct Requete UneRequeteR ;

 memset(&psoo,0,sizeof(struct sockaddr_in)) ;
 memset(&psos,0,sizeof(struct sockaddr_in)) ; 
 memset(&psor,0,sizeof(struct sockaddr_in)) ; 
 
 if (argc!=5)
  
{
    printf("cli client portc serveur ports\n") ;
    exit(1) ;
 }
 Desc = CreateSockets(&psoo,&psos,argv[1],atoi(argv[2]),argv[3],atoi(argv[4])) ;
 if ( Desc == -1 )
    perror("CreateSockets:") ;
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;
 
  Ip = GetIP(&psoo) ;
  Port = GetPort(&psoo) ;
  memset(Tampon,0,sizeof(Tampon)) ;
  Ipv4ToS(Ip, Tampon ) ;
  printf("Adresse Ip du client %s : Port %d \n", Tampon,Port ) ;
 
 Compteur = 0 ;
 while(1)
 {
   /* ici setjmp */
   returned_from_longjump = sigsetjmp(env,1) ; /* <> 0 pour sauver le masque,ne pas utiliser setjmp */
   if ( returned_from_longjump!= 0)
     switch (returned_from_longjump)	 {
	case SIGINT:
	     printf("longjumped from interrupt CRTL C  %d\n",SIGINT);
	     exit(0) ;
	     break;
	case SIGTSTP:
             printf("longjumped from interrupt CTRL Z  %d\n",SIGTSTP);
             break;
	case SIGALRM:
	     printf("longjumped from alarm %d\n",SIGALRM);
	     NbrAlarmes++ ;
	     printf("Alarme numero %d \n",NbrAlarmes);
	     if (NbrAlarmes==4)
	         exit(0) ;
	     break;  
     } ;
   
  signal(SIGINT, signal_handler);
  signal(SIGALRM, signal_handler);
  signal(SIGTSTP, signal_handler) ; 
  time_interval = 9 ;
  ret = alarm(time_interval); 
   UneRequeteE.Compteur = Compteur ;
   UneRequeteE.Type = Question ; 
   strncpy(UneRequeteE.Message , "Hello Multiclient" , sizeof(UneRequeteE.Message)) ;
   rc = SendDatagram(Desc,&UneRequeteE,sizeof(struct Requete) ,&psos ) ;

   if ( rc == -1 )
      perror("SendDatagram") ;
   else
      fprintf(stderr,"Envoi du message %d avec %d bytes\n",UneRequeteE.Compteur, rc ) ;
 
   while(1) /* on boucle tant que l'on ne reçoit pas le bon message */
   {
     memset(&UneRequeteR,0,sizeof(struct Requete)) ;
     tm = sizeof(struct Requete) ;
     rc = ReceiveDatagram( Desc, &UneRequeteR,tm, &psor ) ;
     if (UneRequeteR.Compteur != UneRequeteE.Compteur ) 
        printf("doublon !!!!!\n") ;
     else
        break ;
   }
   /* fin de l'alarme */ 
   ret = alarm(0);
   if ( rc == -1 )
       perror("ReceiveDatagram") ;
   else
   {
      fprintf(stderr,"bytes:%d:Compteur %d:%s\n",rc,UneRequeteR.Compteur,UneRequeteR.Message ) ;
   }
   Compteur++ ;
   sleep(5) ;
 }

 close(Desc) ;
}
