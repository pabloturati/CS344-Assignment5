#include <netinet/in.h>
#ifndef SHAREDMETHODS_H
#define SHAREDMETHODS_H

void exitWithError(const char *, int);
void freeMemoryAndCloseFile(char *, FILE *);

#endif