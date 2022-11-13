#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>

using namespace std;

extern unordered_set<string> resources;
extern unordered_set<string> users_ids;

extern void load_simple_db(string db_file, unordered_set<string> *db); 

#endif // SERVER_DATABASE_H