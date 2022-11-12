#include <iostream>
#include <rpc/rpc.h>

#include "rpc_auth.h"
#include "token.h"

using namespace std;

int main(int argc, char *argv[]) {
    char *token;

    token = generate_access_token("X5B0TWjmeNtU3vd");
    cout << token << "\n";

    token = generate_access_token(token);
    cout << token << "\n";

    // token = generate_access_token(token);
    // cout << token << "\n";

    cout << "Another one \n";
    token = generate_access_token("X5B0TWjmeNtU3vd");
    cout << token << "\n";

    free(token);

    return 0;
}