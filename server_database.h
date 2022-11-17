#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

#include "token.h"

extern std::unordered_set<std::string> resources;
extern std::unordered_set<std::string> users_ids;
extern std::queue<std::string> approvals;
extern int token_ttl;
extern std::unordered_set<std::string> signed_tokens;
extern std::unordered_map<std::string, std::string> users_req_access_tokens; // tine user_id - req_acc_token
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> users_permissions_set;
extern std::unordered_map<std::string, std::string> users_accessed_tokens; // tine user_id - acc_token
extern std::unordered_map<std::string, int> acc_tokens_availibilty; // acc_token -  ttl
extern std::unordered_map<std::string, std::string> da; // tine auth_acc_token -> acc_token
extern std::unordered_map<std::string, std::string> ref_tokens; // tine acc_tokens -> ref_token if needed 

extern void load_simple_db(std::string db_file, std::unordered_set<std::string> *db); 
extern void load_permissions(std::string perm_file, std::queue<std::string> *approvals);
extern void load_token_ttl(std::string ttl_file);
extern std::unordered_map<std::string, std::vector<int>> get_user_files_permissions();
extern bool find_acc_token_user(std::string token);
extern bool is_operation_allowed(int op_type, std::string resource, std::string acc_token);
extern std::string find_user_by_acc_token(std::string token);
extern void refresh_acc_token(std::string expired_acc_token);
extern std::string find_key(std::string token);

#endif // SERVER_DATABASE_H