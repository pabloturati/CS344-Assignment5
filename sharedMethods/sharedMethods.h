#include <netinet/in.h>
#ifndef SHAREDMETHODS_H
#define SHAREDMETHODS_H

void exitWithError(const char *, int);
void freeMemoryAndCloseFile(char *, FILE *);
void validateArgCount(int, int, char *);
char *createDynamicFilenameWithProcessId(char *);
void exitWithDinamicallyGeneratedMessage(char *, char *);

FILE *openFile(char *, char *);
FILE *openFileForReading(char *);
FILE *openFileForWriting(char *);
FILE *openFileForReadingAndWriting(char *);
int deleteFile(char *);
void inspectForSocketReadError(int);
void handleReceiveData(int);

#endif