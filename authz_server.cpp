#include <stdio.h>
#include <iostream>
#include <string>

#include "rpc_auth.h"
#include "server_database.h"
#include "utils.h"
#include "token.h"

using namespace std;

resp_req_authorization_t *request_authorization_1_svc(req_authorization_t *data, struct svc_req *cl) {
    resp_req_authorization_t *resp =(resp_req_authorization_t*) malloc(sizeof(resp_req_authorization_t));

    cout << "BEGIN " << data->user_id << " AUTHZ" << endl;

    if (!data) {
        cout << "Null data" << endl;
        exit(1);
    } 

    if (users_ids.find(data->user_id) == users_ids.end()) {
        resp->status = USER_NOT_FOUND;
        resp->token = "";
        return resp;
    }
    resp->status = OK;
    resp->token = generate_access_token(data->user_id);

    cout << "\tRequestToken=" << resp->token << endl;

    return resp;
}

server_response *approve_request_token_1_svc(req_access_auth_token_t *data, struct svc_req *cl){
    server_response *resp = (server_response*)malloc(sizeof(server_response));
    string permissions;
    unordered_map<string, vector<int>> user_permission = get_user_files_permissions();

    users_permissions_set[data->req_access_auth_token] = user_permission;

    if (user_permission.find("*") != user_permission.end()) {
        resp->response = DENY;
        return resp;
    }

    signed_tokens.insert(data->req_access_auth_token);

    resp->response = APPROVE;
	return resp;
}

resp_req_access_token_t *request_access_token_1_svc(req_access_token_t *data, struct svc_req *cl) {
	resp_req_access_token_t *resp = (resp_req_access_token_t*)malloc(sizeof(resp_req_access_token_t));

    if (signed_tokens.find(data->req_access_auth_token) == signed_tokens.end()) {
        resp->status = REQUEST_DENIED;
        return resp;
    }

    resp->status = OK;
    resp->acc_token = generate_access_token(data->req_access_auth_token);
    resp->ref_token = "";
    if (data->generate_ref_token) {
        resp->ref_token = generate_access_token(resp->acc_token);
    }

    resp->token_avalibilty_time = 1;

    cout << "\tAccessToken=" << resp->acc_token << endl; 

    users_accessed_tokens[data->user_id] = resp->acc_token;
    
    return resp;
}

server_response *validate_delegated_action_1_svc(delegated_action_t *, struct svc_req *) {
	return NULL;
}
