
enum TypeRequete {
       Question = 1 ,
       Reponse  = 2 
     } ;

struct Requete
{
  enum TypeRequete Type ;
  char Message[40] ; 
} ; 