#ifndef ENCRYPTIONMETHODS_H
#define ENCRYPTIONMETHODS_H

int createRandomNumberInRange(int, int);
int isValidCharacter(char c);
int encodeChar(char c);
char encryptChar(char, char);
char decryptChar(char, char);

#endif