#include <iostream>
#include <fstream>
#include <string.h>
#include <rpc/rpc.h>
#include "token.h"

#include "server_database.h"
#include "rpc_auth_svc.c"

using namespace std;

// extern unordered_set<string> users_ids;

server_response *request_authorization_1_svc(req_authorization_t *, struct svc_req *) {
    return NULL;
}

permissions_set *approve_request_token_1_svc(req_access_auth_token_t *, struct svc_req *) {
	return NULL;
}

server_response *request_access_token_1_svc(req_access_token_t *, struct svc_req *) {
	return NULL;
}

server_response *validate_delegated_action_1_svc(delegated_action_t *, struct svc_req *) {
	return NULL;
}

int main (int argc, char **argv)
{

    // if (argc != 4) {
    //     cout << "Wrong number of arguments!" << endl;
    //     exit(1);
    // }

    load_simple_db(argv[1], &users_ids);
    load_simple_db(argv[2], &resources);

    register SVCXPRT *transp;

    pmap_unset (AUTH_PROG, AUTH_VERS);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
            fprintf (stderr, "%s", "cannot create udp service.");
            exit(1);
    }
    if (!svc_register(transp, AUTH_PROG, AUTH_VERS, auth_prog_1, IPPROTO_UDP)) {
        fprintf (stderr, "%s", "unable to register (AUTH_PROG, AUTH_VERS, udp).");
        exit(1);
    }

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
            fprintf (stderr, "%s", "cannot create tcp service.");
            exit(1);
    }
    if (!svc_register(transp, AUTH_PROG, AUTH_VERS, auth_prog_1, IPPROTO_TCP)) {
        fprintf (stderr, "%s", "unable to register (AUTH_PROG, AUTH_VERS, tcp).");
        exit(1);
    }

    svc_run ();
    fprintf (stderr, "%s", "svc_run returned");
    exit (1);
    /* NOTREACHED */
}

/*
g++ -I/usr/include/tirpc/ -o server -g server.cpp   rpc_auth_xdr.c  -lnsl -ltirpc 

*/