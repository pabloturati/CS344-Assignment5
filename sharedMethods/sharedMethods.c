#include <stdlib.h>
#include <stdio.h>
#include "./sharedMethods.h"

void exitWithError(const char *msg, int errorCode)
{
  perror(msg);
  exit(errorCode);
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
  // printf("Upper is %d\n", upperLimit);
  // printf("Lower is %d\n", lowerLimit);
  int random = rand() % (upperLimit - lowerLimit + 1);
  // printf("Rand is %d\n", lowerLimit);
  return random + lowerLimit;
}