#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <queue>

using namespace std;

extern unordered_set<string> resources;
extern unordered_set<string> users_ids;
extern queue<string> users_approvals;

extern void load_simple_db(string db_file, unordered_set<string> *db); 
extern void load_permissions(string perm_file, queue<string> *perm);

#endif // SERVER_DATABASE_H