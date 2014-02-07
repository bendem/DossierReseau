
enum TypeRequete {
    Question = 1,
    Reponse  = 2
};



struct RequeteBDEF {
    enum TypeRequete Type;
    int NumTransac;
    int NumeroTicket;
    enum Action Action;
};
