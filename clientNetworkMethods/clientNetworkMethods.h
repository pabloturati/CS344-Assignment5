#ifndef CLIENTNETWORKMETHODS_H
#define CLIENTNETWORKMETHODS_H

void setupClientAddressStruct(struct sockaddr_in *, int, char *);
int createClientSocketAndConnectServer(int);

#endif