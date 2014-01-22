
enum TypeRequete {
       Question = 1 ,
       Reponse  = 2 
     } ;

struct Requete
{
  enum TypeRequete Type ;
  int  Compteur ;
  char Message[80] ; 
} ; 