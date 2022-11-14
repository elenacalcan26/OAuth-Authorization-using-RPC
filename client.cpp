#include <iostream>
#include <rpc/rpc.h>

#include "rpc_auth.h"
#include "utils.h"

#define SERVER_ADDRESS		"localhost"
#define RPC_AUTH_PROG			1
#define RPC_AUTH_VERS			1
#define PROTOCOL			"tcp"

using namespace std;

int main(int argc, char *argv[]) {
    CLIENT *cl;
    req_authorization_t req_authorization_data;
    resp_req_authorization_t *resp;
    server_response *is_approved;
    req_access_auth_token_t req_acc_auth_token;
    resp_req_access_token_t *resp_acc_token;
    req_access_token_t req_acc_token;


    if (argc != 1) {
        cout << "Wrong number of parameters" << endl;
        exit(1);
    }

    cl = clnt_create(SERVER_ADDRESS, RPC_AUTH_PROG, RPC_AUTH_VERS, PROTOCOL);

    if (cl == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(2);
    }

    string id = "X5B0TWjmeNtU3vd";
    req_authorization_data.user_id = strdup(id.c_str());

    resp = request_authorization_1(&req_authorization_data, cl);

    if (resp == NULL) {
        cout << "null\n";
        exit(1);
    }

    req_acc_auth_token.req_access_auth_token = resp->token;

    is_approved = approve_request_token_1(&req_acc_auth_token, cl);

    if (is_approved->response == DENY) {
        cout << "DENIED";
    } else {
        
        req_acc_token.user_id = strdup(id.c_str());
        req_acc_token.req_access_auth_token = resp->token;

        resp_acc_token = request_access_token_1(&req_acc_token, cl);

        cout << "bbb\n";

        cout << resp_acc_token->acc_token << endl;
    }

  
    free(req_authorization_data.user_id);
    free(req_acc_token.user_id);
    clnt_destroy(cl);

    return 0;
}
