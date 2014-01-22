
#include <stdio.h>
#include <string.h>


char* SuffixeDe(char *Suffixe, char* Chaine)
{
  int LongSuffixe,LongChaine ;
 // on compare les chaines à partir de la fin
 LongSuffixe=strlen(Suffixe) ;
 LongChaine=strlen(Chaine) ;
 if (LongSuffixe == 0 || LongChaine == 0 )
     return NULL ;
 Suffixe= Suffixe + LongSuffixe-1 ;
 Chaine= Chaine + LongChaine-1  ;
  while ( LongChaine && LongSuffixe && *Suffixe==*Chaine  )
  {
   LongSuffixe-- ;
   LongChaine-- ;
   Suffixe-- ;
   Chaine-- ;
  }
  
  if (LongSuffixe)
     return NULL ;
  else
      return (Chaine+1) ;
}
  
int main(int argc,char* argv[])
{
  char * res ;
  printf("Suffixe %s \n",argv[1]) ;
  printf("Chaine  %s \n",argv[2]) ;
  res = SuffixeDe(argv[1],argv[2]) ;
  if ( res==NULL )
     printf("Non Trouvé !!!!!\n") ;
  else
     printf(" Resultat %s \n", SuffixeDe(argv[1],argv[2]) ) ;
}
  
