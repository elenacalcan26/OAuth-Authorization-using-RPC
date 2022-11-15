struct req_authorization_t {
    string user_id<>;
};

struct resp_req_authorization_t {
    int status;
    string token<>;
};

struct resp_req_access_token_t {
    int status;
    string acc_token<>;
    string ref_token<>;
    int token_avalibilty_time;
};

struct req_access_token_t {
    string user_id<>;
    string req_access_auth_token<>;
    int generate_ref_token;
};

struct server_response {
    int response;
};

struct delegated_action_t {
    string operation_type<>;
    string resource<>;
    string access_token<>;
};

struct req_access_auth_token_t {
    string req_access_auth_token<>;
};

program AUTH_PROG {
    version AUTH_VERS {
        resp_req_authorization_t request_authorization(req_authorization_t) = 1;
        resp_req_access_token_t request_access_token(req_access_token_t) = 2;
        server_response validate_delegated_action(delegated_action_t) = 3;
        server_response approve_request_token(req_access_auth_token_t) = 4;
    } = 1;
} = 1;