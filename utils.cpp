#include "utils.h"

using namespace std;

char* generate_access_token(char* clientIdToken) {
    char *token = (char*)malloc(TOKEN_LEN * sizeof(char*));
    int i, key, used[TOKEN_LEN];
    int rotationIndex = TOKEN_LEN;

    memset(used, 0, TOKEN_LEN * sizeof(int));
    for (i = 0; i < TOKEN_LEN; i++) {
        do {
            key = rand() % rotationIndex;
        } while (used[key] == 1);
        token[i] = clientIdToken[key];
        used[key] = 1;
    }
    token[TOKEN_LEN] = '\0';
    return token;
}

vector<string> split_comma_seprated_str(string str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    setbuf(stdout, NULL);

    while (getline(ss, token, COMMA_SEP))
    {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());

        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

string find_str_key(string val, unordered_map<string, string> map) {

    for (auto &elem : map) {
        if (!val.compare(elem.second)) {
            return elem.first;
        }
    }

    // nu s-a gasit cheia asociata valorii data ca si parametru
    return "";
}

unordered_set<string> decode_permissions(string permissions) {
    unordered_set<string> decoded_perms;

    for (char const &ch : permissions) {
        switch (ch) {
            case 'R': 
                decoded_perms.insert("READ"); 
                break;
            case 'I': 
                decoded_perms.insert("INSERT"); 
                break;
            case 'M': 
                decoded_perms.insert("MODIFY"); 
                break;
            case 'D': 
                decoded_perms.insert("DELETE"); 
                break;
            case 'X': 
                decoded_perms.insert("EXECUTE"); 
                break;
            default: break; 
        }
    }
    return decoded_perms;
} 

char* decode_action_response(int code) {
    switch (code)
    {
    case USER_NOT_FOUND:
        return (char*)"USER_NOT_FOUND";
        break;
    case REQUEST_DENIED:
        return (char*)"REQUEST_DENIED";
        break;
    case PERMISSION_DENIED:
        return (char*)"PERMISSION_DENIED";
        break;
    case TOKEN_EXPIRED:
        return (char*)"TOKEN_EXPIRED";
        break;
    case RESOURCE_NOT_FOUND:
        return (char*)"RESOURCE_NOT_FOUND";
        break;
    case OPERATION_NOT_PERMITTED:
        return (char*)"OPERATION_NOT_PERMITTED";
        break;
    case PERMISSION_GRANTED:
        return (char*)"PERMISSION_GRANTED";
        break; 
    default:
        break;
    }

    return (char*)"PERMISSION_GRANTED";
}
