#include <iostream>
#include <rpc/rpc.h>
#include <fstream>

#include "rpc_auth.h"
#include "utils.h"

#define SERVER_ADDRESS		"localhost"
#define RPC_AUTH_PROG			1
#define RPC_AUTH_VERS			1
#define PROTOCOL			"tcp"

using namespace std;

vector<string> get_tokens(string str) {
    vector<string> tokens;
    stringstream ss(str);
    char sep = ',';
    string token;

    while (getline(ss, token, sep))
    {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());

        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

int main(int argc, char *argv[]) {
    CLIENT *cl;
    ifstream file;
    string line;
    vector<string> operations_params;

    // req_authorization_t req_authorization_data;
    // resp_req_authorization_t *resp;
    // server_response *is_approved;
    // req_access_auth_token_t req_acc_auth_token;
    // resp_req_access_token_t *resp_acc_token;
    // req_access_token_t req_acc_token;

    if (argc != 2) {
        cout << "Wrong number of parameters" << endl;
        exit(1);
    }

    cl = clnt_create(SERVER_ADDRESS, RPC_AUTH_PROG, RPC_AUTH_VERS, PROTOCOL);

    if (cl == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(2);
    }

    file.open(argv[1]);

    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        exit(1);
    }

    while (getline(file, line)) {
        operations_params = get_tokens(line);

        string user_id = operations_params[0];
        string operation_type = operations_params[1];

        if (operation_type.compare("REQUEST") == 0) {
            req_authorization_t req_authorization_data;
            resp_req_authorization_t *resp_auth;
            server_response *is_approved;
            req_access_auth_token_t req_acc_auth_token;
            req_access_token_t req_acc_token;
            resp_req_access_token_t *resp_acc_token;

            req_authorization_data.user_id = strdup(user_id.c_str());
            resp_auth = request_authorization_1(&req_authorization_data, cl);

            if (resp_auth->status == USER_NOT_FOUND) {
                cout << "USER NOT FOUND" << endl;
                continue;
            }

            req_acc_auth_token.req_access_auth_token = resp_auth->token;
            is_approved = approve_request_token_1(&req_acc_auth_token, cl);

            if (is_approved->response == DENY) {
                cout << "REQUEST_DENIED" << endl;
                continue;
            }

            req_acc_token.user_id = strdup(user_id.c_str());
            req_acc_token.req_access_auth_token = resp_auth->token;

            resp_acc_token = request_access_token_1(&req_acc_token, cl);

            cout << resp_auth << " -> " << resp_acc_token->acc_token << endl;

            free(req_authorization_data.user_id);
            // free(resp_auth);
            // free(resp_acc_token);
            // free(is_approved);
        }
    }

    file.close();
    clnt_destroy(cl);

    return 0;
}
