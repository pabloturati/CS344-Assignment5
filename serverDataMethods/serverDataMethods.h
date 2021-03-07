#ifndef SERVERDATAMETHODS_H
#define SERVERDATAMETHODS_H
#include <stdlib.h>
#include <stdio.h>

void verifyKeyAndFileSizesMatch(int, int);
void writeEncryptedFile(FILE *, FILE *, FILE *);
int printFileContents(FILE *fp);
void writeTempRawTextAndKeyFiles(int, FILE *, FILE *);
void closeAndDeleteFile(FILE *, char *);

#endif