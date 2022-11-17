#include "server_database.h"
#include "utils.h"

using namespace std;

unordered_set<string> users_ids;
unordered_set<string> resources;
queue<string> approvals;
int token_ttl;
unordered_set<string> signed_tokens;
unordered_map<string, unordered_map<string, vector<int>>> users_permissions_set;
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

vector<int> decode_permissions(string permissions) {
    vector<int> decoded_perms;

    for (char const &ch : permissions) {
        switch (ch) {
            case 'R': 
                decoded_perms.push_back(READ); 
                break;
            case 'I': 
                decoded_perms.push_back(INSERT); 
                break;
            case 'M': 
                decoded_perms.push_back(MODIFY); 
                break;
            case 'D': 
                decoded_perms.push_back(DELETE); 
                break;
            case 'X': 
                decoded_perms.push_back(EXECUTE); 
                break;
            default: decoded_perms.push_back(-1); // no permission
        }
    }
    return decoded_perms;
} 

// ia din coada permisiunile pe care le are un user asupra resurselor
unordered_map<string, vector<int>> get_user_files_permissions() {
    unordered_map<string, vector<int>> files_permissions;
    string user_files_permissions = approvals.front();
    vector<string> tokens = get_tokens(user_files_permissions);

    for (int i = 0; i < tokens.size(); i += 2) {
        string file = tokens[i];
        string file_perms = tokens[i + 1];

        files_permissions[file] = decode_permissions(file_perms);
    }
    
    approvals.pop();
    return files_permissions;
}

bool find_acc_token_user(std::string token) {
    for (auto &it : users_accessed_tokens) {
        if (it.second.compare(token) == 0) {
            return true;
        }
    }
    
    return false;
}

string find_key(std::string token) {
    for (auto &it : da) {
        if (it.second.compare(token) == 0) {
            return it.first;
        }
    }
    
    return "";
}

string find_user_by_acc_token(string token) {
    for (auto &it : users_accessed_tokens) {
        if (it.second.compare(token) == 0) {
            return it.first;
        } 
    }

    return "";
}

bool is_operation_allowed(int op_type, string resource, string acc_token) {
    string req_acc_token = find_key(acc_token);
    unordered_map<string, vector<int>> user_files = users_permissions_set[req_acc_token]; 
    vector<int> permissions = user_files[resource];
    for (auto &i : permissions) {
        if (i == op_type) {
            return true;
        }
    }
    
    return false;
}
