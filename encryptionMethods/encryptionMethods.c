#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../constants/constants.h"

/*
Calculates integer encoding for character 
Input: msg-> value to encrypt (string), key-> encryption secret (string) 
Output:  0 for A, 25 for Z, 26 for space character
*/
int encodeChar(char c)
{
  return c == SPACE_CHAR ? SPACE_RAND_VAL : c - ASCII_VAL_OF_A;
}

/*
Function to enconde using a key of the same length as text
Input: msg-> value to encrypt (string), key-> encryption secret (string) 
  size -> string size (integer). Size must be identical for msg and key.
Output: returns encripted message of the same length as key/message
*/
char *encryptionHandler(char *msg, char *key, int size)
{
  char *encryptedMsg = (char *)calloc(size, sizeof(char));
  int i = 0;
  for (; i < size; i++)
  {
    int cypherChar = (encodeChar(msg[i]) + encodeChar(key[i])) % TOTAL_AMOUNT_OF_CHARS;
    //if cypheredChar equals 26 (space character)
    encryptedMsg[i] = cypherChar == SPACE_RAND_VAL ? ASVCII_VAL_OF_SPACE : cypherChar + ASCII_VAL_OF_A;
  }
  encryptedMsg[i] = '\0';
  return encryptedMsg;
}

/*
Function to create a random number in the range
Input: void
Output: returns random number
Refrence:  Adapted from code from https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
 and https://www.geeksforgeeks.org/generating-random-number-range-c/
*/
int createRandomNumberInRange(int lowerLimit, int upperLimit)
{
  return (rand() % (upperLimit - lowerLimit + 1)) + lowerLimit;
}