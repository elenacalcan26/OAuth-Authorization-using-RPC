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

using namespace std;

extern unordered_set<string> resources;
extern unordered_set<string> users_ids;
extern queue<string> approvals;
extern unordered_set<string> signed_tokens;
extern unordered_map<string, unordered_map<string, vector<int>>> users_permissions_set;
extern unordered_map<string, string> users_accessed_tokenes;

extern void load_simple_db(string db_file, unordered_set<string> *db); 
extern void load_permissions(string perm_file, queue<string> *approvals);
extern unordered_map<string, vector<int>> get_user_files_permissions();


#endif // SERVER_DATABASE_H