/*-----------------------------------------------------------------------------
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Vanstapel Herman EPL

 physlib\physlib.h
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ----------------------------------------------------------------------------*/

#ifndef PHYSLIB
#define PHYSLIB
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "../tcplib/tcplib.h"
#include <errno.h>

extern int errno ;

int CreateSockets(struct sockaddr_in *psoo,struct sockaddr_in *psos,char * NCli,int PortCli,char *NSer,int PortSer) ;
int SendDatagram(int desc,void *message,int tm, struct sockaddr_in *psos ) ;
int ReceiveDatagram(int desc, void *message,int tm, struct sockaddr_in *psor ) ;
int GetIP(struct sockaddr_in *pin) ;
int GetPort(struct sockaddr_in *pin) ;
void DelNewLine(char *Chaine) ;
char ReadChar() ;

#endif
