/*---------------------------------------------------------------------------------------------
  Herman vanstapel

 ex04\ser.c :  Un serveur avec plusieurs clients qui affiche chaque fois l'ip et le port du client connecté
-----------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../physlib/physlib.h"
#include "structure.h"
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>

sigjmp_buf env; 

static void signal_handler(int sig)
{  
   siglongjmp(env,sig);
}

int main(int argc,char *argv[])
{
 int rc ;
 int Desc ;
 struct sockaddr_in psoo ; /* o = origine */
 struct sockaddr_in psos ; /* s = serveur */
 struct sockaddr_in psor ; /* r = remote */
 
 int tm ;
 struct Requete UneRequete ;
 char Tampon[40] ;
 int Ip, Port ;

 memset(&psoo,0,sizeof(struct sockaddr_in)) ;
 memset(&psos,0,sizeof(struct sockaddr_in)) ; 
 memset(&psor,0,sizeof(struct sockaddr_in)) ; 
 
 printf("Ceci est le serveur\n") ;
 if ( argc!=3)
 {
  printf("ser ser port \n") ;
  exit(1) ;
 }
 
 Desc = CreateSockets(&psoo,NULL,argv[1],atoi(argv[2]),NULL,0) ;
 if ( Desc == -1 )
    perror("CreateSockets") ;
 else
    printf(" CreateSockets : %d \n",Desc) ;
 
 while(1) 
 {
  int returned_from_longjump ;
   returned_from_longjump = sigsetjmp(env,1) ; /* <> 0 pour sauver le masque,ne pas utiliser setjmp */
   if ( returned_from_longjump!= 0)
     switch (returned_from_longjump)	 {
	case SIGINT:
	     printf("longjumped from interrupt CRTL C  %d\n",SIGINT);
             /* On fermerait les fichiers */
	     exit(0) ;
	     break;
	case SIGTSTP:
             printf("longjumped from interrupt CTRL Z  %d\n",SIGTSTP);
             printf("Demarrage du sleep \n") ;
             sleep(30) ;
             printf("Fin du sleep \n") ;
             break ;
     } ;

  signal(SIGINT, signal_handler);
  signal(SIGTSTP, signal_handler) ; 
  memset(&UneRequete,0,sizeof(UneRequete)) ;
  tm = sizeof(struct Requete) ;
  rc = ReceiveDatagram( Desc,&UneRequete ,tm, &psor ) ;
  if ( rc == -1 )
    perror("ReceiveDatagram") ;
  else
     fprintf(stderr,"bytes:%d:%s:%d\n",rc,UneRequete.Message,UneRequete.Compteur); 
  /***********************************************************************
  int	Ipv4ToInt(char *s,int *ip) ;
  void	Ipv4ToS(int ip, char *s) ; 
  int GetIP(struct sockaddr_in *pin) ;
  int GetPort(struct sockaddr_in *pin) ;
  ***********************************************************************/
 
   
  Ip = GetIP(&psor) ;
  Port = GetPort(&psor) ;
  memset(Tampon,0,sizeof(Tampon)) ;
  Ipv4ToS(Ip, Tampon ) ;
  
  printf("Adresse Ip du client %s : Port %d \n", Tampon,Port ) ;
  /* reponse avec psor qui contient toujours l'adresse du dernier client */
 
  UneRequete.Type = Reponse ; 
  strcat(UneRequete.Message," Client") ;
  rc = SendDatagram(Desc,&UneRequete,sizeof(struct Requete) ,&psor ) ;
  if ( rc == -1 )
    perror("SendDatagram:") ;
  else
    fprintf(stderr,"bytes:%d\n",rc ) ;
 }
}
