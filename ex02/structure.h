enum TypeRequete {
   Question = 1,
   Reponse  = 2
};

struct RequeteBDEF {
    TypeRequete Type;
    int NumeroTicket;
    int NumTransac;
    Action Action;
};
