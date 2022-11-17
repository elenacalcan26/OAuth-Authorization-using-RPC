#include <stdio.h>
#include <iostream>
#include <string>

#include "rpc_auth.h"
#include "server_database.h"
#include "utils.h"

using namespace std;

resp_req_authorization_t *request_authorization_1_svc(req_authorization_t *data, struct svc_req *cl) {
    resp_req_authorization_t *resp =(resp_req_authorization_t*) malloc(sizeof(resp_req_authorization_t));

    cout << "BEGIN " << data->user_id << " AUTHZ" << endl;

    if (!data) {
        cout << "Null data" << endl;
        exit(1);
    } 

    // verifca existenta user-ului in DB
    if (users_ids.find(data->user_id) == users_ids.end()) {
        resp->status = USER_NOT_FOUND;
        resp->token = "";
        return resp;
    }

    // caz fericit, user exista si ii genreaza token-ul pentru cererea de acces la resurse
    resp->status = OK;
    resp->token = generate_access_token(data->user_id);

    users_req_access_tokens[data->user_id] = resp->token;

    cout << "  RequestToken = " << resp->token << endl;

    return resp;
}

server_response *approve_request_token_1_svc(req_access_auth_token_t *data, struct svc_req *cl){
    server_response *resp = (server_response*)malloc(sizeof(server_response));
    string permissions;
    unordered_map<string, vector<int>> user_permission = get_user_files_permissions();

    // for (auto &file : user_permission) {
    //     cout << file.first << " -> ";

    //     for (auto &perms : user_permission[file.first]) {
    //         cout << perms << " ";
    //     }

    //     cout << endl;
    // }

    // se ataseaza permisiunile user-ului la token-ul pentru cerea de acces la resurse
    users_permissions_set[data->req_access_auth_token] = user_permission;

    // verific daca user-ul aproba sau nu permisiunile
    if (user_permission.find("*") != user_permission.end()) {
        resp->response = DENY;
        return resp;
    }

    // user-ul aproba permisiunile si token-ul este semnat
    signed_tokens.insert(data->req_access_auth_token);

    resp->response = APPROVE;
	return resp;
}

resp_req_access_token_t *request_access_token_1_svc(req_access_token_t *data, struct svc_req *cl) {
	resp_req_access_token_t *resp = (resp_req_access_token_t*)malloc(sizeof(resp_req_access_token_t));

    if (signed_tokens.find(data->req_access_auth_token) == signed_tokens.end()) {
        // jetonul pentru cerere de acces la resurse nu este semnat => REQUET_DENIED
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
    da[data->req_access_auth_token] = resp->acc_token;
    
    return resp;
}

resp_req_access_token_t *req_refresh_acc_token_1_svc(req_refresh_acc_token_t *data, struct svc_req *cl) {
    resp_req_access_token_t *resp = (resp_req_access_token_t*)malloc(sizeof(resp_req_access_token_t));
    string user_id = data->user_id;
    string old_acc_token = data->expired_acc_token;
    string old_ref_token = ref_tokens[old_acc_token];

    cout << "BEGIN " << user_id << " AUTHZ REFRESH" << endl;

    resp->acc_token = generate_access_token((char*)old_ref_token.c_str());
    resp->ref_token = generate_access_token(resp->acc_token);

    cout << "  AccessToken = " << resp->acc_token << endl;
    cout << "  RefreshToken = " << resp->ref_token << endl;

    resp->status = OK;
    resp->token_avalibilty_time = token_ttl;

    // users_accessed_tokens -> modify this with new acc token
    users_accessed_tokens[user_id] = resp->acc_token;

    // acc_tokens_availibilty -> remove old acc_token, add the ne one + ttl
    acc_tokens_availibilty.erase(old_acc_token);
    acc_tokens_availibilty[resp->acc_token] = token_ttl;

    // da -> also, replace old acc_token with the new ones
    string req_auth_token = find_key(old_acc_token);
    da[req_auth_token] = resp->acc_token;

    // si sterge vechea perche, cu cele 2 noi token-uri generate acum
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
