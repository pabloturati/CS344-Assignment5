#include <netinet/in.h>
#ifndef CLIENTDATAMETHODS_H
#define CLIENTDATAMETHODS_H

int validateCharacters(char *);
int sendFileToServer(char *, int, int);
int validateTextFileAndKey(char *, char *);
void handleSendFilesToServer(int, char *, char *, int (*)(int));

#endif