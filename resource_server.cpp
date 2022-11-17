#include <stdio.h>
#include <iostream>
#include <string>

#include "rpc_auth.h"
#include "server_database.h"
#include "utils.h"

using namespace std;

char *uncode_op_type(int op_type) {
    switch (op_type)
    {
    case 1:
        return (char*)"READ";
        break;
    case 2:
        return (char*)"INSERT";
        break;
    case 3:
        return (char*)"MODIFY";
        break;
    case 4:
        return (char*)"DELETE";
        break;
    case 5:
        return (char*)"EXECUTE";
        break;
    default:
        break;
    }

    return "";
}

void print_action_msg(string action, int op_type, string resource, string acc_token) {
    cout << action  << " (" 
    << uncode_op_type(op_type) << "," 
    << resource <<","
    << acc_token << ","
    << acc_tokens_availibilty[acc_token] << ")"
    << endl;
}

server_response *validate_delegated_action_1_svc(delegated_action_t *data, struct svc_req *cl) {
	server_response *resp = (server_response*)malloc(sizeof(server_response));
    int op_type = data->operation_type;
    string resource = data->resource;
    string acc_token = data->access_token;
    
    // verifca daca jetonul de acces este asociat unui user
    if (find_acc_token_user(acc_token) == false) {
        print_action_msg("DENY", op_type, resource, acc_token);
        resp->response = PERMISSION_DENIED;
        return resp;    
    }

    // verifica daca token-ul de acces este inca valabil
    if (acc_tokens_availibilty[acc_token] <= 0) {
        resp->response = TOKEN_EXPIRED;
        string user_id = find_user_by_acc_token(acc_token);
        print_action_msg("DENY", op_type, resource, "");
        return resp;
    }

    // verifica daca resursa este disponibila in server
    if (resources.find(resource) == resources.end()) {
        resp->response = RESOURCE_NOT_FOUND;
        acc_tokens_availibilty[acc_token]--;
        print_action_msg("DENY", op_type, resource, acc_token);
        return resp;
    }

    // verifca daca operatia este permisa prin jetonul de acces
    if (!is_operation_allowed(op_type, resource, acc_token)) {
        resp->response = OPERATION_NOT_PERMITTED;
        acc_tokens_availibilty[acc_token]--;
        print_action_msg("DENY", op_type, resource, acc_token);
        return resp;
    }

    // caz fericit
    acc_tokens_availibilty[acc_token]--;
    print_action_msg("PERMIT", op_type, resource, acc_token);
    resp->response = OK;
    return resp;
}
