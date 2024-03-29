#include <stdio.h>
#include <iostream>
#include <string>

#include "rpc_auth.h"
#include "server_database.h"

using namespace std;

resp_req_authorization_t *request_authorization_1_svc(char **data, struct svc_req *cl) {
    resp_req_authorization_t *resp =(resp_req_authorization_t*) malloc(sizeof(resp_req_authorization_t));

    cout << "BEGIN " << *data << " AUTHZ" << endl;

    if (!data) {
        cout << "Null data" << endl;
        exit(1);
    } 

    // verifca existenta user-ului in DB
    if (users_ids.find(*data) == users_ids.end()) {
        resp->status = USER_NOT_FOUND;
        resp->token = "";
        return resp;
    }

    // user exista si ii genreaza token-ul pentru cererea de acces la resurse
    resp->status = OK;
    resp->token = generate_access_token(*data);

    users_req_access_tokens[*data] = resp->token;

    cout << "  RequestToken = " << resp->token << endl;

    return resp;
}

int *approve_request_token_1_svc(char **data, struct svc_req *cl){
    int *resp = (int*)malloc(sizeof(int));
    string permissions;
    unordered_map<string, unordered_set<string>> user_permission = get_user_files_permissions();

    // se ataseaza permisiunile user-ului la token-ul pentru cerea de acces la resurse
    users_permissions_set[*data] = user_permission;

    // verific daca user-ul aproba sau nu permisiunile
    if (user_permission.find("*") != user_permission.end()) {
        *resp = DENY;
        return resp;
    }

    // user-ul aproba permisiunile si token-ul este semnat
    signed_tokens.insert(*data);

    *resp = APPROVE;
	return resp;
}

resp_req_access_token_t *request_access_token_1_svc(req_access_token_t *data, struct svc_req *cl) {
	resp_req_access_token_t *resp = (resp_req_access_token_t*)malloc(sizeof(resp_req_access_token_t));

    if (signed_tokens.find(data->req_access_auth_token) == signed_tokens.end()) {
        // jetonul pentru cerere de acces la resurse nu este semnat => REQUEST_DENIED
        resp->status = REQUEST_DENIED;
        return resp;
    }

    resp->status = OK;
    // se genereaza token-ul de acces la resurse
    resp->acc_token = generate_access_token(data->req_access_auth_token);
    cout << "  AccessToken = " << resp->acc_token << endl; 
    
    resp->ref_token = "";
    if (data->generate_ref_token) {
        // se genereaza refresh token la cererea user-ului
        resp->ref_token = generate_access_token(resp->acc_token);

        // adauga token-ul de refresh a user-ului
        ref_tokens[resp->acc_token] = resp->ref_token;
        cout << "  RefreshToken = " << resp->ref_token << endl;
    }

    resp->token_avalibilty_time = token_ttl;

    // se asociaza acces token user-ului
    users_accessed_tokens[data->user_id] = resp->acc_token;

    // se seteaza timpul token-ului de acces
    acc_tokens_availibilty[resp->acc_token] = token_ttl;

    // asociaza token-ul cererii de acces la resurse cu token-ul de acces
    auth_acc_tokens[data->req_access_auth_token] = resp->acc_token;
    
    return resp;
}

resp_req_access_token_t *req_refresh_acc_token_1_svc(req_refresh_acc_token_t *data, struct svc_req *cl) {
    resp_req_access_token_t *resp = (resp_req_access_token_t*)malloc(sizeof(resp_req_access_token_t));
    string user_id = data->user_id;
    string old_acc_token = data->expired_acc_token;
    string old_ref_token = ref_tokens[old_acc_token];

    cout << "BEGIN " << user_id << " AUTHZ REFRESH" << endl;

    // se genereaza noi token-uri
    resp->acc_token = generate_access_token((char*)old_ref_token.c_str());
    resp->ref_token = generate_access_token(resp->acc_token);

    cout << "  AccessToken = " << resp->acc_token << endl;
    cout << "  RefreshToken = " << resp->ref_token << endl;

    resp->status = OK;
    resp->token_avalibilty_time = token_ttl;

    users_accessed_tokens[user_id] = resp->acc_token;

    // token-urile vechi sunt suprascrise cu cele noi create
    acc_tokens_availibilty.erase(old_acc_token);
    acc_tokens_availibilty[resp->acc_token] = token_ttl;
    string req_auth_token = find_str_key(old_acc_token, auth_acc_tokens);
    auth_acc_tokens[req_auth_token] = resp->acc_token;
    ref_tokens.erase(old_acc_token);
    ref_tokens[resp->acc_token] = resp->ref_token;
    
    return resp;
}

int *get_acc_token_ttl_1_svc(char **data, struct svc_req *cl) {
    int *resp = (int*)malloc(sizeof(int));
    string acc_token = *data;
    *resp = acc_tokens_availibilty[acc_token];
    return resp;
}
