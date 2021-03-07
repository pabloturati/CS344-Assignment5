#ifndef SERVERNETWORKMETHODS_H
#define SERVERNETWORKMETHODS_H

void setupServerAddressStruct(struct sockaddr_in *, int);
int createServerSocketAndListenForConnections(int);
int acceptClientConnection(int);

#endif