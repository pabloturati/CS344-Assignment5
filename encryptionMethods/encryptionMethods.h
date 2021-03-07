#ifndef ENCRYPTIONMETHODS_H
#define ENCRYPTIONMETHODS_H

int createRandomNumberInRange(int, int);
char *encryptionHandler(char *, char *, int);
char *decryptHandler(char *, char *, int);
int isValidCharacter(char c);
int encodeChar(char c);
char encryptChar(char, char);

#endif