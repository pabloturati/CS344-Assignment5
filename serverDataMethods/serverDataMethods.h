#ifndef SERVERDATAMETHODS_H
#define SERVERDATAMETHODS_H
#include <stdlib.h>
#include <stdio.h>

void verifyKeyAndFileSizesMatch(int, int);
int countTextLength(FILE *);
void writeTempRawTextAndKeyFiles(int, FILE *, FILE *);
void closeAndDeleteFile(FILE *, char *);
void handleServerFileProcess(int, void (*)(FILE *, FILE *, FILE *));
void transformData(FILE *, FILE *, FILE *, char (*)(char, char));
void sendFileUsingSocket(int, FILE *, const int);

#endif