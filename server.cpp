#include <iostream>
#include <fstream>
#include <string.h>
#include <rpc/rpc.h>
#include "token.h"

#include "rpc_auth.h"

using namespace std;

int main (int argc, char **argv)
{
        register SVCXPRT *transp;

        pmap_unset (CHECK_PROG, CHECK_VERS);

        transp = svcudp_create(RPC_ANYSOCK);
        if (transp == NULL) {
                fprintf (stderr, "%s", "cannot create udp service.");
                exit(1);
        }
        if (!svc_register(transp, CHECK_PROG, CHECK_VERS, check_prog_1, IPPROTO_UDP)) {
                fprintf (stderr, "%s", "unable to register (CHECK_PROG, CHECK_VERS, udp).");
                exit(1);
        }

        transp = svctcp_create(RPC_ANYSOCK, 0, 0);
        if (transp == NULL) {
                fprintf (stderr, "%s", "cannot create tcp service.");
                exit(1);
        }
        if (!svc_register(transp, CHECK_PROG, CHECK_VERS, check_prog_1, IPPROTO_TCP)) {
                fprintf (stderr, "%s", "unable to register (CHECK_PROG, CHECK_VERS, tcp).");
                exit(1);
        }

        svc_run ();
        fprintf (stderr, "%s", "svc_run returned");
        exit (1);
        /* NOTREACHED */
}
