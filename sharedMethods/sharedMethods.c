#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../constants/constants.h"

void exitWithError(const char *msg, int errorCode)
{
  fprintf(stderr, "%s\n", msg);
  exit(errorCode);
}

void freeMemoryAndCloseFile(char *line, FILE *fp)
{
  free(line);
  fclose(fp);
}