struct req_authorization_t {
    string user_id<>;
};

struct resp_req_authorization_t {
    int status;
    string token<>;
};

struct req_access_token_t {
    string user_id<>;
    string req_access_auth_token<>;
};

struct server_response {
    int response<>;
};

struct delegated_action_t {
    string operation_type<>;
    string resource<>;
    string access_token<>;
};

struct resource_permissions {
    int status;
    string resource<>;
    int permissions<>;
};

struct permissions_set {
    resource_permissions permissions<>;
};

struct req_access_auth_token_t {
    string req_access_auth_token<>;
};

program AUTH_PROG {
    version AUTH_VERS {
        resp_req_authorization_t request_authorization(req_authorization_t) = 1;
        server_response request_access_token(req_access_token_t) = 2;
        server_response validate_delegated_action(delegated_action_t) = 3;
        permissions_set approve_request_token(req_access_auth_token_t) = 4;
    } = 1;
} = 1;