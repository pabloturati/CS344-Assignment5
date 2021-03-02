#include <netinet/in.h>
#ifndef SHAREDMETHODS_H
#define SHAREDMETHODS_H

void exitWithError(const char *, int);
void setupServerAddressStruct(struct sockaddr_in *, int);
void setupClientAddressStruct(struct sockaddr_in *, int, char *);

#endif