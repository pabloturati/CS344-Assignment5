#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants/constants.h"
#include "sharedMethods/sharedMethods.h"
#include "encryptionMethods/encryptionMethods.h"

int main(int argc, const char *argv[])
{
  if (argc < 2)
  {
    exitWithError(KEY_GEN_TOO_FEW_ARGS, 1);
  }
  int strLen = atoi(argv[1]);
  srand(time(0));

  int randNum;
  for (int i = 0; i < strLen; i++)
  {
    randNum = createRandomNumberInRange(LOWER_RAND_RANGE, UPPER_RAND_RANGE);
    fprintf(stdout, "%c", randNum == SPACE_RAND_VAL ? SPACE_CHAR : randNum + ASCII_VAL_OF_A);
  }
  fprintf(stdout, "\n");

  return 0;
}