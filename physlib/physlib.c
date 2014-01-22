
/*-----------------------------------------------------------------------------
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Vanstapel Herman EPL

 \physlib\physlib.c
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ----------------------------------------------------------------------------*/
#include "physlib.h"
#include "../tcplib/tcplib.h"

/* Structure d'une sockaddr_in 

  pin->sin_family      = AF_INET ;
  pin->sin_addr.s_addr = *ai  ; adresse internet
  pin->sin_port        = htons(port) ;

 */


//int CreerConnexion(struct Physique *pL ,char * NCli,int pcli,char *NSer,int pser )
int CreateSockets(struct sockaddr_in *psoo,struct sockaddr_in *psos,char * NCli,int PortCli,char *NSer,int PortSer) 
{
 int res ;
 int desc ;
 char *message ;
 struct hostent *phcli,*phser ;
 int ip4cl,ip4se ; /*adresse du client et serveur */

 phcli = gethostbyname(NCli) ;
 if ( phcli == NULL )
    {
     perror("Nom client\n");
     return (-1) ;
    }
 memcpy(&(ip4cl), phcli->h_addr_list[0],sizeof(ip4cl)) ;
 fprintf(stderr,"IP du client:") ;
 afficher_adresse((struct ip4* )&(ip4cl) ) ;

 /*----- adresse IP du serveur ---*/
 if (NSer != NULL )
 {
  phser = gethostbyname(NSer) ;
  if ( phser == NULL )
     {
      perror("Nom Serveur\n");
      return(-1) ;
     }
  memcpy(&(ip4se), phser->h_addr_list[0],sizeof(ip4se)) ;
  fprintf(stderr,"IP du serveur:") ;
  afficher_adresse((struct ip4* )&(ip4se) ) ;
 }

 /* Creation socket Client */
 desc=creer_socket(SOCK_DGRAM,(u_long *)&(ip4cl) ,PortCli,psoo);
 if (desc==-1)
    return(desc) ;
 else
    fprintf(stderr,"Creation socket réussie \n") ;

 /* Creation socket Serveur */
 if (NSer != NULL )
 {
  psos-> sin_family = AF_INET ;
  memcpy( &(psos->sin_addr.s_addr),&(ip4se),4) ;
  psos-> sin_port = htons(PortSer) ;
 }
 return(desc) ;
}

/* struct sockaddr_in sain ; */

int SendDatagram(int desc,void *message,int tm, struct sockaddr_in *psos )
{
  return(sendto(desc,message,tm,0,(struct sockaddr *) psos,sizeof(struct sockaddr_in))) ;
}

int ReceiveDatagram(int desc, void *message,int tm, struct sockaddr_in *psor )
{
 /* psor contiendra l'adresse de l'émetteur */
 unsigned int ta ;
 ta = sizeof(struct sockaddr_in) ;
 return(recvfrom(desc,message,tm,0,(struct sockaddr *) psor,&ta)) ;
}

int GetIP(struct sockaddr_in *pin)
{
 int ip ;
 ip = ntohl(pin->sin_addr.s_addr) ;
 return(ip)  ;

}

int GetPort(struct sockaddr_in *pin)
{
 int port ;
 port = ntohs(pin->sin_port) ;
 return(port)  ;
}

/* Fonctions clavier */

void DelNewLine(char *Chaine)
{
 Chaine[strlen(Chaine)-1] = 0 ;
}

char ReadChar()
{
 char Tampon[80] ;
 fgets(Tampon,sizeof Tampon,stdin ) ;
 return Tampon[0] ;
} 

