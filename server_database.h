#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <algorithm>

#include "utils.h"

extern std::unordered_set<std::string> resources;
extern std::unordered_set<std::string> users_ids;
extern std::queue<std::string> approvals;
extern int token_ttl;
extern std::unordered_set<std::string> signed_tokens;

// Map-uri folosite pentru gestionarea informatiilor si relatiilor intre ele

// tine user_id - request_authentification_access_token
extern std::unordered_map<std::string, std::string> users_req_access_tokens;

// request_access_token - setul de permisiuni a fisierelor
extern std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> users_permissions_set;

//user_id - access_token 
extern std::unordered_map<std::string, std::string> users_accessed_tokens;

// access_token - perioda_de_valabilitate
extern std::unordered_map<std::string, int> acc_tokens_availibilty;

// request_authentification_access_token -> access_token
extern std::unordered_map<std::string, std::string> auth_acc_tokens; 

// access_token -> refresh_token
extern std::unordered_map<std::string, std::string> ref_tokens;

extern void load_simple_db(std::string db_file, std::unordered_set<std::string> *db); 
extern void load_permissions(std::string perm_file, std::queue<std::string> *approvals);
extern void load_token_ttl(std::string ttl_file);
extern std::unordered_map<std::string, std::unordered_set<std::string>> get_user_files_permissions();
extern bool is_operation_allowed(std::string op_type, std::string resource, std::string acc_token);
extern void refresh_acc_token(std::string expired_acc_token);

#endif // SERVER_DATABASE_H