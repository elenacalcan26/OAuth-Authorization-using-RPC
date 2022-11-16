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

extern std::unordered_set<std::string> resources;
extern std::unordered_set<std::string> users_ids;
extern std::queue<std::string> approvals;
extern int token_ttl;
extern std::unordered_set<std::string> signed_tokens;
extern std::unordered_map<std::string, std::string> users_req_access_tokens;
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> users_permissions_set;
extern std::unordered_map<std::string, std::string> users_accessed_tokens;
extern std::unordered_map<std::string, int> acc_tokens_availibilty;

extern void load_simple_db(std::string db_file, std::unordered_set<std::string> *db); 
extern void load_permissions(std::string perm_file, std::queue<std::string> *approvals);
extern void load_token_ttl(std::string ttl_file);
extern std::unordered_map<std::string, std::vector<int>> get_user_files_permissions();


#endif // SERVER_DATABASE_H