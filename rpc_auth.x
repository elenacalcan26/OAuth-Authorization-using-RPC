
/* Contine raspunsul server-ului la cererea de autorizare a user-ului */
struct resp_req_authorization_t {
    /*
    Indica statusul cererii
        OK -> cerere efectuata cu succes
        USER_NOT_FOUND -> eroare, user-ul nu este gasit in BD
    */
    int status;

    /* jeton pentru autorizarea cererii de acces la resurse */
    string token<>;
};

/* Contine respunsul server-ului pentru cererea jetonului de acces la resurse */
struct resp_req_access_token_t {
    /*
    Indica statusul cererii
        OK -> cerere efectuata cu succes
        REQUEST_DENIED -> eroare, jeton invalid
    */
    int status;

    /* jetonul de acces la resurse */
    string acc_token<>;

    /* jetonul de refresh*/
    string ref_token<>;

    /* periada de valabilitate a jetonului de acces */
    int token_avalibilty_time;
};

/* Contine informatiile user-ului pentru cererea token-ului de acces */
struct req_access_token_t {
    /* ID-ul user-ului */
    string user_id<>;

    /* token-ul pentru cererea de acces la resurse */
    string req_access_auth_token<>;

    /* indica daca user-ul a optat sau nu pentru reinnoire automata a jetonului */
    int generate_ref_token;
};

/* Contine informatiile cererii de efectuare a unei operatii pe care user-ul vrea sa o faca */
struct delegated_action_t {
    /* tipul opertiei */
    string operation_type<>;
    
    /* resursa asupra careia i se face operatia */
    string resource<>;

    /* jetonul de acces la resurse */
    string access_token<>;
};

struct req_access_auth_token_t {
    string req_access_auth_token<>;
};

/* Contine informatiile pentru cererea user-ului de a reinnoi jetonul de acces */
struct req_refresh_acc_token_t {
    /* ID-ul user-ului */
    string user_id<>;

    /* jetonul care va fi reinnoit */
    string expired_acc_token<>;
};

program AUTH_PROG {
    version AUTH_VERS {
        /* cerere de autorizare a user-ului */
        resp_req_authorization_t request_authorization(string) = 1;
        
        /* cerere pentru jetonul de acces la resurse */
        resp_req_access_token_t request_access_token(req_access_token_t) = 2;
        
        /* cerere pentru efectuarea unei operatii asupra unei resurse */
        int validate_delegated_action(delegated_action_t) = 3;
        
        /* cererea pentru aprobarea accesului la resurse */
        int approve_request_token(string) = 4;

        /* cerere pentru reinnoirea token-ului de acces */
        resp_req_access_token_t req_refresh_acc_token(req_refresh_acc_token_t) = 5;
        
        /* cerere pentru aflarea timpul de valabilitate a token-ului */
        int get_acc_token_ttl(string) = 6;
    } = 1;
} = 1;
