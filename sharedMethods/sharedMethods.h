#include <netinet/in.h>
#ifndef SHAREDMETHODS_H
#define SHAREDMETHODS_H

void exitWithError(const char *, int);
void freeMemoryAndCloseFile(char *, FILE *);
void validateArgCount(int, int, char *);

#endif