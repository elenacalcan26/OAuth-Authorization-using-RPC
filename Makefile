RPC = rpcgen
CC = g++
LDFLAGS = -I/usr/include/tirpc/ -lnsl -ltirpc

build:
	$(RPC) -m rpc_auth.x -o rpc_auth_svc.c
	$(RPC) -l rpc_auth.x -o rpc_auth_clnt.c 
	$(RPC) -c rpc_auth.x -o rpc_auth_xdr.c
	$(RPC) -h rpc_auth.x -o rpc_auth.h
	$(CC) -o server -g  server.cpp server_database.cpp  authz_server.cpp resource_server.cpp rpc_auth_xdr.c  utils.cpp $(LDFLAGS)
	$(CC) -o client -g  client.cpp rpc_auth_clnt.c rpc_auth_xdr.c utils.cpp $(LDFLAGS)

clean:
	rm server client rpc_auth_xdr.c rpc_auth_clnt.c rpc_auth_svc.c  rpc_auth.h