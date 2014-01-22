/************************************************************************************************************

   Ex06 : Herman Vanstapel 

   Programme permettant de tester les gestionnaires de signaux

  Vous ppuvez aussi tester les signauc autre par la combinaison
  
  kill -s USR1  pid

******************************************************************************************************************/

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>  /* ces deux include pour le getpid */
#include <unistd.h>
#include <stdlib.h>

jmp_buf env; 
int mask ;
static void signal_handler(int sig);
main()
{
 int returned_from_longjump, processing = 1;
 int count = 0 ;
 static unsigned int time_interval = 5 ; 
 int ret ;
 struct sigaction new_action, old_action;
 
 printf("Affichage du pid %d \n",getpid()) ;
 returned_from_longjump = sigsetjmp(env,1) ; /* <> 0 pour sauver le masque , ne surtout pas utiliser setjmp */
 if ( returned_from_longjump!= 0)
     switch (returned_from_longjump)	 {
	case SIGINT:
	     printf("longjumped from interrupt %d\n",SIGINT);
	     break;
	case SIGALRM:
	     printf("longjumped from alarm %d\n",SIGALRM);
	     printf("Valeur de time interval %d \n",time_interval);
	     break;
	default :
	     printf("longjumped from  unknow alarm %d\n",returned_from_longjump);
	     break;  
     } ;
 
  (void) signal(SIGINT, signal_handler);
 (void ) signal(SIGALRM, signal_handler); 
  time_interval = 5 ;
  ret = alarm(time_interval);
  printf(" valeur de ret :%d \n",ret ) ;
    count = 0 ;
    while (1)	
    {
		   printf(" waiting for you to INTERRUPT (cntrl-C)  %d ...\n", count );
		   pause() ; /* eviter le sleep qui peut implémenter alarm */
		   perror("resultat") ;
    }  /* end while forever loop */
}

static void signal_handler(int sig)
{
 switch (sig)	
 {
   case SIGINT:     /* process for interrupt */
   
					siglongjmp(env,sig);
	                                break ;   		
   
					      /* break never reached */
   case SIGALRM:    /* process for alarm */
     
				         siglongjmp(env,sig);
					break ;
     					      /* break never reached */
   default:		siglongjmp(env,sig);
					break ;
 }
}
