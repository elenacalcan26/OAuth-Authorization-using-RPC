#include <stdio.h>
#include <iostream>
#include <string>

#include "rpc_auth.h"
#include "server_database.h"

using namespace std;

void print_action_msg(string action, string op_type, string resource, string acc_token) {
    cout << action  << " (" 
    << op_type << "," 
    << resource <<","
    << acc_token << ","
    << acc_tokens_availibilty[acc_token] << ")"
    << endl;
}

int *validate_delegated_action_1_svc(delegated_action_t *data, struct svc_req *cl) {
	int *resp = (int *)malloc(sizeof(int));
    string resource = data->resource;
    string acc_token = data->access_token;
    string op_type = data->operation_type;
    
    // verifca daca jetonul de acces este asociat unui user
    if (!find_str_key(acc_token, users_accessed_tokens).compare("")) {
        print_action_msg("DENY", op_type, resource, acc_token);
        *resp = PERMISSION_DENIED;
        return resp;    
    }

    // verifica daca token-ul de acces este inca valabil
    if (acc_tokens_availibilty[acc_token] <= 0) {
        *resp = TOKEN_EXPIRED;
        string user_id = find_str_key(acc_token, users_accessed_tokens);
        print_action_msg("DENY", op_type, resource, "");
        return resp;
    }

    // verifica daca resursa este disponibila in server
    if (resources.find(resource) == resources.end()) {
        *resp = RESOURCE_NOT_FOUND;
        acc_tokens_availibilty[acc_token]--;
        print_action_msg("DENY", op_type, resource, acc_token);
        return resp;
    }

    // verifca daca operatia este permisa prin jetonul de acces
    if (!is_operation_allowed(op_type, resource, acc_token)) {
        *resp = OPERATION_NOT_PERMITTED;
        acc_tokens_availibilty[acc_token]--;
        print_action_msg("DENY", op_type, resource, acc_token);
        return resp;
    }

    // caz fericit
    acc_tokens_availibilty[acc_token]--;
    print_action_msg("PERMIT", op_type, resource, acc_token);
    *resp = OK;
    return resp;
}
