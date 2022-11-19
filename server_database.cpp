#include "server_database.h"

using namespace std;

unordered_set<string> users_ids;
unordered_set<string> resources;
queue<string> approvals;
int token_ttl;
unordered_set<string> signed_tokens;
unordered_map<string, unordered_map<std::string, unordered_set<string>>> users_permissions_set;
unordered_map<string, string> users_accessed_tokens;
unordered_map<string, int> acc_tokens_availibilty;
unordered_map<string, string> users_req_access_tokens;
unordered_map<string, string> da;
std::unordered_map<std::string, std::string> ref_tokens;

void load_simple_db(string db_file, unordered_set<string> *db) {
    ifstream file;
    file.open(db_file);
    string id;
    int num_users;

    if (!file.is_open()) {
        cout << "Can't open file!\n";
        exit(1);
    }

    file >> num_users;

    while (file)
    {
        getline(file, id);
        (*db).insert(id);
    }
    
    file.close();
}

void load_permissions(string perm_file, queue<string> *approvals) {
    ifstream file;
    file.open(perm_file);
    string approval;

    if (!file.is_open()) {
        cout << "Can't open file!\n";
        exit(1);
    }

    while (file)
    {
        getline(file, approval);
        (*approvals).push(approval);
    }
    
    file.close();
}

void load_token_ttl(std::string ttl_file) {
    ifstream file;
    file.open(ttl_file);
    file >> token_ttl;
    file.close();
}

// ia din coada permisiunile pe care le are un user asupra resurselor
unordered_map<string, unordered_set<string>> get_user_files_permissions() {
    unordered_map<string, unordered_set<string>> files_permissions;
    string user_files_permissions = approvals.front();
    vector<string> tokens = split_comma_seprated_str(user_files_permissions);

    for (int i = 0; i < tokens.size(); i += 2) {
        string file = tokens[i];
        string file_perms = tokens[i + 1];

        files_permissions[file] = decode_permissions(file_perms);
    }
    
    approvals.pop();
    return files_permissions;
}

bool is_operation_allowed(string op_type, string resource, string acc_token) {
    string req_acc_token = find_str_key(acc_token, da);
    unordered_map<string, unordered_set<string>> user_files = users_permissions_set[req_acc_token]; 
    unordered_set<string> permissions = user_files[resource];
    
    return (permissions.find(op_type) != permissions.end());
}
