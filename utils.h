#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string.h>
#include <algorithm>
#include <sstream>

#define OK 0
#define USER_NOT_FOUND 1
#define REQUEST_DENIED 2
#define PERMISSION_DENIED 3
#define TOKEN_EXPIRED 4
#define RESOURCE_NOT_FOUND 5
#define OPERATION_NOT_PERMITTED 6
#define PERMISSION_GRANTED 7

// permissions
#define READ 1
#define INSERT 2
#define MODIFY 3
#define DELETE 4
#define EXECUTE 5

// user approvals
#define APPROVE 0
#define DENY 1

#endif // UTILS_H
