#include <iostream>
#include <fstream>
#include <string.h>
#include <rpc/rpc.h>

#include "server_database.h"
#include "rpc_auth_svc.c"

using namespace std;

int main (int argc, char **argv)
{

    if (argc != 5) {
        cout << "Wrong number of arguments!" << endl;
        exit(1);
    }

	setbuf(stdout, NULL);

    load_simple_db(argv[1], &users_ids);
    load_simple_db(argv[2], &resources);
	load_permissions(argv[3], &approvals);
	load_token_ttl(argv[4]);
	
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
