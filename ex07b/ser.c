/*--------------------------------------
  Herman Vanstapel 
  ex07 ser.c 
  Un serveur multiport multiclients
----------------------------------------*/

#include <stdio.h>
#include <string.h>
/* include pour le select */
#include <sys/types.h>
#include <sys/time.h>

#include "../physlib/physlib.h"
#include "structure.h"

void TraitementRequete(int Desc )
{
  struct sockaddr_in psor ; /* r = remote */
  
  int tm ;
 struct Requete UneRequete ;
 char Tampon[40] ;
 int Ip, Port ;
 int rc ;
 
 printf("Traitement requete sur %d \n",Desc ) ;
 memset(&psor,0,sizeof(struct sockaddr_in)) ; 
 tm = sizeof(struct Requete) ;
  rc = ReceiveDatagram( Desc,&UneRequete ,tm, &psor ) ;
  if ( rc == -1 )
    perror("ReceiveDatagram") ;
  else
     fprintf(stderr,"bytes:%d:%s num: %d\n",rc,UneRequete.Message,UneRequete.Compteur ) ;
  /***********************************************************************
  int	Ipv4ToInt(char *s,int *ip) ;
  void	Ipv4ToS(int ip, char *s) ; 
  int GetIP(struct sockaddr_in *pin) ;
  int GetPort(struct sockaddr_in *pin) ;
  ***********************************************************************/
 
  scanf("%d",&rc) ; 
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


int main(int argc,char *argv[])
{
 int rc ;
 int Desc1 ;
 char car ;
 struct sockaddr_in psoo1 ; /* o = origine */
 struct sockaddr_in psos1 ; /* s = serveur */
 
 fd_set readfs ;
 struct timeval tv ;

 memset(&psoo1,0,sizeof(struct sockaddr_in)) ;
 memset(&psos1,0,sizeof(struct sockaddr_in)) ;  
 
 printf("Ceci est le serveur\n") ;
 if ( argc!=3)
 {
  printf("ser ser port \n") ;
  exit(1) ;
 }
 
 Desc1 = CreateSockets(&psoo1,&psos1,argv[1],atoi(argv[2]),NULL,0) ;
 if ( Desc1 == -1 )
    perror("CreateSockets") ;
 else
    printf(" CreateSockets 1 : %d \n",Desc1) ;
 
 while(1) 
 {
   
   FD_ZERO(&readfs) ;
   FD_SET(0,     &readfs); /* on teste le clavier */
   FD_SET(Desc1, &readfs);
   
   tv.tv_sec = 30 ; /* 30 secondes de timeout */
   tv.tv_usec = 100000 ; /* temps en micro secondes */
    
   // FD_SETSIZE Taille Maximum du set , Conseil de Doms Etienne */
   if((rc = select( FD_SETSIZE, &readfs, NULL, NULL, &tv)) < 0)
     {
      perror("select()");
      exit(errno);    
     }
    else
    if ( rc == 0 )
        printf ("Timeout !!!!!!! \n" ) ;
    else
    if(FD_ISSET( 0, &readfs))
     {
       car =  ReadChar() ;  /* fonction bibliothèque */
       printf("La touche pressée est %c \n", car  ) ;
       if (( car == 'q') || ( car == 'Q'))
	   exit(0) ;
       if (( car == 's') || ( car == 'S'))
       {
	char Tampon[80] ;
	int Duree ;
	printf("Durée:") ;
	fgets( Tampon, sizeof(Tampon) , stdin ) ;
	Duree = atoi(Tampon ) ;
	sleep(Duree) ;
	printf(" Fin Duree \n") ;
       }
     }
    else
    if(FD_ISSET(Desc1, &readfs))
    {
      TraitementRequete(Desc1 ) ;      
    }
 }
}
