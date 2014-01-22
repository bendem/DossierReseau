/*--------------------------------------
  cphex\ser.c 
  ex01 un seveur recevant des des chaines de caractères lié à un seul client
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../physlib/physlib.h"

int main(int argc,char *argv[])
{
 int rc ;
 int Desc ;
 struct sockaddr_in psoo ; /* o = origine */
 struct sockaddr_in psos ; /* s = serveur */
 struct sockaddr_in psor ; /* r = remote */

 char message[100] ;
 char *reponse ;
 
 int tm ;

 memset(&psoo,0,sizeof(struct sockaddr_in)) ;
 memset(&psos,0,sizeof(struct sockaddr_in)) ; 
 memset(&psor,0,sizeof(struct sockaddr_in)) ; 
 
 printf("Ceci est le serveur\n") ;
 if ( argc!=5)
 {
  printf("ser ser port cli port\n") ;
  exit(1) ;
 }
 
 Desc = CreateSockets(&psoo,&psos,argv[1],atoi(argv[2]),argv[3],atoi(argv[4])) ;
 if ( Desc == -1 )
    perror("CreateSockets") ;
 else
    printf(" CreateSockets : %d \n",Desc) ;
 
 tm = sizeof(message) ;
 rc = ReceiveDatagram( Desc, message,tm, &psor ) ;
 if ( rc == -1 )
    perror("ReceiveDatagram") ;
 else
   fprintf(stderr,"bytes:%d:%s\n",rc,message ) ;
 
 /* reponse avec psos */
 reponse = "bonjour client 1" ;
 rc = SendDatagram(Desc,reponse,strlen(reponse)+1 ,&psos ) ;
 if ( rc == -1 )
    perror("SendDatagram") ;
 else
   fprintf(stderr,"bytes:%d\n",rc ) ;
 
 /* reponse avex psor r = remote */
 reponse = "bonjour client 2" ;
 rc = SendDatagram(Desc,reponse,strlen(reponse)+1 ,&psor ) ;
 if ( rc == -1 )
    perror("Sendatagram") ;
 else
   fprintf(stderr,"bytes:%d\n",rc ) ;
}
