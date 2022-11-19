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
    ifstream file;
    string line;
    vector<string> operations_params;
    unordered_map<string, string> users_auth_tokens;
    unordered_map<string, string> users_acc_tokens;
    unordered_map<string, int> users_refresh_req; 

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
        operations_params = split_comma_seprated_str(line);

        string user_id = operations_params[0];
        string operation_type = operations_params[1];

        if (operation_type.compare("REQUEST") == 0) {
            req_authorization_t req_authorization_data;
            resp_req_authorization_t *resp_auth;
            server_response *is_approved;
            req_access_auth_token_t req_acc_auth_token;
            req_access_token_t req_acc_token;
            resp_req_access_token_t *resp_acc_token;

            int generate_ref_token = stoi(operations_params[2]);
            users_refresh_req[user_id] = generate_ref_token;

            req_authorization_data.user_id = strdup(user_id.c_str());
            resp_auth = request_authorization_1(&req_authorization_data, cl);

            if (resp_auth->status == USER_NOT_FOUND) {
                cout << "USER_NOT_FOUND" << endl;
                continue;
            }

            req_acc_auth_token.req_access_auth_token = resp_auth->token;
            is_approved = approve_request_token_1(&req_acc_auth_token, cl);

            if (is_approved->response == DENY) {
                cout << "REQUEST_DENIED" << endl;
                continue;
            }

            users_auth_tokens[user_id] = resp_auth->token;

            req_acc_token.user_id = strdup(user_id.c_str());
            req_acc_token.req_access_auth_token = resp_auth->token;
            req_acc_token.generate_ref_token = generate_ref_token;

            resp_acc_token = request_access_token_1(&req_acc_token, cl);

            cout << resp_auth->token << " -> " << resp_acc_token->acc_token;

            if (strcmp(resp_acc_token->ref_token, "")) {
                cout << "," << resp_acc_token->ref_token;
            }

            cout << endl;

            users_acc_tokens[user_id] = resp_acc_token->acc_token;

            free(req_authorization_data.user_id);
        } else {
            // verifica daca valabilitatea token-ului si daca poate face cerere de refresh
            char *user_acc_token = (char*)users_acc_tokens[user_id].c_str();
            int* acc_token_ttl = get_acc_token_ttl_1(&user_acc_token, cl);

            if (users_refresh_req[user_id] == 1 && *acc_token_ttl <= 0) {
                req_refresh_acc_token_t req_ref;
                req_ref.expired_acc_token = user_acc_token;
                req_ref.user_id = (char*)user_id.c_str();

                resp_req_access_token_t *resp_ref = req_refresh_acc_token_1(&req_ref, cl);

                users_acc_tokens[user_id] = resp_ref->acc_token;

            }

            string resource = operations_params[2];
            server_response *is_valid;
            delegated_action_t action;

            action.access_token = strdup(users_acc_tokens[user_id].c_str());
            action.operation_type = strdup(operation_type.c_str());
            action.resource = strdup(resource.c_str());
            
            is_valid = validate_delegated_action_1(&action, cl);

            cout << decode_action_response(is_valid->response) << endl;
        }
    }

    file.close();
    clnt_destroy(cl);

    return 0;
}
