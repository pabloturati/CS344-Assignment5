#ifndef SERVERDATAMETHODS_H
#define SERVERDATAMETHODS_H
#include <stdlib.h>
#include <stdio.h>

void verifyKeyAndFileSizesMatch(int, int);
int printFileContents(FILE *fp);
void writeTempRawTextAndKeyFiles(int, FILE *, FILE *);
void closeAndDeleteFile(FILE *, char *);
void handleServerFileProcess(int, void (*f)(FILE *, FILE *, FILE *));

#endif