#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#define TOKEN_LEN 15

#define OK 0
#define USER_NOT_FOUND 1
#define REQUEST_DENIED 2
#define PERMISSION_DENIED 3
#define TOKEN_EXPIRED 4
#define RESOURCE_NOT_FOUND 5
#define OPERATION_NOT_PERMITTED 6
#define PERMISSION_GRANTED 7

// user approvals
#define APPROVE 0
#define DENY 1

#define COMMA_SEP ','

char* generate_access_token(char* clientIdToken);
std::vector<std::string> split_comma_seprated_str(std::string str);
std::string find_str_key(std::string val, std::unordered_map<std::string, std::string> map);
std::unordered_set<std::string> decode_permissions(std::string permissions);
char* decode_action_response(int code);

#endif // UTILS_H
