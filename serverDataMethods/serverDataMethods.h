#ifndef SERVERDATAMETHODS_H
#define SERVERDATAMETHODS_H
#include <stdlib.h>
#include <stdio.h>

void verifyKeyAndFileSizesMatch(int, int);
int printFileContents(FILE *);
void writeTempRawTextAndKeyFiles(int, FILE *, FILE *);
void closeAndDeleteFile(FILE *, char *);
void handleServerFileProcess(int, void (*)(FILE *, FILE *, FILE *));
void transformData(FILE *, FILE *, FILE *, char (*)(char, char));

#endif