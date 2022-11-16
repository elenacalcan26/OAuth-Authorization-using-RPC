#include <iostream>
#include <rpc/rpc.h>
#include <fstream>
#include <unordered_map>

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

    setbuf(stdout, NULL);

    while (getline(ss, token, sep))
    {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());

        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

int decode_operation(string op) {
    if (op.compare("READ") == 0) {
        return READ;
    } else if (op.compare("INSERT") == 0) {
        return INSERT;
    } else if (op.compare("MODIFY") == 0) {
        return MODIFY;
    } else if (op.compare("DELETE") == 0) {
        return DELETE;
    } else if (op.compare("EXECUTE") == 0) {
        return EXECUTE;
    }

    return -1;
}

char* decode_action_response(int code) {
    switch (code)
    {
    case USER_NOT_FOUND:
        return "USER_NOT_FOUND";
        break;
    case REQUEST_DENIED:
        return "REQUEST_DENIED";
        break;
    case PERMISSION_DENIED:
        return "PERMISSION_DENIED";
        break;
    case TOKEN_EXPIRED:
        return "TOKEN_EXPIRED";
        break;
    case RESOURCE_NOT_FOUND:
        return "RESOURCE_NOT_FOUND";
        break;
    case OPERATION_NOT_PERMITTED:
        return "OPERATION_NOT_PERMITTED";
        break;
    case PERMISSION_GRANTED:
        return "PERMISSION_GRANTED";
        break; 
    default:
        break;
    }

    return "PERMISSION_GRANTED";
}

int main(int argc, char *argv[]) {
    CLIENT *cl;
    ifstream file;
    string line;
    vector<string> operations_params;
    unordered_map<string, string> users_acc_tokens;

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

            int generate_ref_token = stoi(operations_params[2]);

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

            req_acc_token.user_id = strdup(user_id.c_str());
            req_acc_token.req_access_auth_token = resp_auth->token;
            req_acc_token.generate_ref_token = generate_ref_token;

            resp_acc_token = request_access_token_1(&req_acc_token, cl);

            cout << resp_auth->token << " -> " << resp_acc_token->acc_token << endl;

            users_acc_tokens[user_id] = resp_acc_token->acc_token;

            free(req_authorization_data.user_id);
            // free(resp_auth);
            // free(resp_acc_token);
            // free(is_approved);
        } else {
            string resource = operations_params[2];
            server_response *is_valid;
            delegated_action_t action;

            action.access_token = strdup(users_acc_tokens[user_id].c_str());
            action.operation_type = decode_operation(operation_type);
            action.resource = strdup(resource.c_str());
            
            is_valid = validate_delegated_action_1(&action, cl);

            cout << decode_action_response(is_valid->response) << endl;
        }
    }

    file.close();
    clnt_destroy(cl);

    return 0;
}
