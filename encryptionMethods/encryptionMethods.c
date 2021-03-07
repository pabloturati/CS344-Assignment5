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
Function to encrypt a message using a key of the same length as text
Input: msg-> value to encrypt (string), key-> encryption secret (string) 
  size -> string size (integer). Size must be identical for msg and key.
Output: returns encripted message of the same length as key/message (string *)
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
  encryptedMsg[i] = END_STRING_CHARACTER;
  return encryptedMsg;
}

/*
Function to encrypt a message using a key of the same length as text
Input: msg-> value to encrypt (string), key-> encryption secret (string) 
  size -> string size (integer). Size must be identical for msg and key.
Output: returns encripted message of the same length as key/message (string *)
*/
char encryptChar(char rawChar, char keyChar)
{
  int encryptedChar = (encodeChar(rawChar) + encodeChar(keyChar)) % TOTAL_AMOUNT_OF_CHARS;
  //if cypheredChar equals 26 (space character)
  return encryptedChar == SPACE_RAND_VAL ? ASVCII_VAL_OF_SPACE : encryptedChar + ASCII_VAL_OF_A;
}

/*
Function to dencrypt a message using a key of the same length as text
Input: msg-> value to encrypt (string), key-> encryption secret (string) 
  size -> string size (integer). Size must be identical for msg and key.
Output: returns encripted message of the same length as key/message (string *)
*/
char *decryptHandler(char *msg, char *key, int size)
{
  char *decryptedMsg = (char *)calloc(size, sizeof(char));
  int i = 0;
  for (; i < size; i++)
  {
    int decodedVal = encodeChar(msg[i]) - encodeChar(key[i]);
    if (decodedVal < 0)
    {
      decodedVal += TOTAL_AMOUNT_OF_CHARS;
    }
    else
    {
      decodedVal %= TOTAL_AMOUNT_OF_CHARS;
    }
    //if cypheredChar equals 26 (space character)
    decryptedMsg[i] = decodedVal == SPACE_RAND_VAL ? ASVCII_VAL_OF_SPACE : decodedVal + ASCII_VAL_OF_A;
  }
  decryptedMsg[i] = END_STRING_CHARACTER;
  return decryptedMsg;
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

/*
Function to validate that a specific character is between A-Z or space
Input: c-> character to validate (char)
Output: returns encripted message of the same length as key/message
*/

int isValidCharacter(char c)
{
  return (c >= ASCII_VAL_OF_A && c <= ASCII_VAL_OF_Z) || c == ASVCII_VAL_OF_SPACE;
}
