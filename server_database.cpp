#include "server_database.h"

unordered_set<string> users_ids;
unordered_set<string> resources;

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

