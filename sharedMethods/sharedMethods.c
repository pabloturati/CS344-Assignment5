#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include "../constants/constants.h"

void exitWithError(const char *msg, int errorCode)
{
  fprintf(stderr, "%s", msg);
  exit(errorCode);
}

void freeMemoryAndCloseFile(char *line, FILE *fp)
{
  free(line);
  fclose(fp);
}

void validateArgCount(int argc, int expectedArgc, char *errorMsg)
{
  if (argc < expectedArgc)
    exitWithError(errorMsg, DEFAULT_ERROR_EXIT_CODE);
}

void exitWithDinamicallyGeneratedMessage(char *errorMsg, char *msgParam)
{
  char errorMsgBuffer[DYNAMIC_ERROR_MESSAGE_MAX_LENGTH];
  sprintf(errorMsgBuffer, errorMsg, msgParam);
  exitWithError(errorMsgBuffer, DEFAULT_ERROR_EXIT_CODE);
}

char *createDynamicFilenameWithProcessId(char *filenamePrefix)
{
  char *tempFilename = (char *)calloc(TEMP_FILENAME_SIZE, sizeof(char));
  snprintf(tempFilename, TEMP_FILENAME_SIZE, filenamePrefix, getpid());
  return tempFilename;
}

FILE *openFile(char *filename, char *permission)
{
  FILE *fd = fopen(filename, permission);
  if (fd == NULL)
    exitWithError(ERROR_OPENING_FILE_MSG, EXIT_FAILURE);
  return fd;
}

FILE *openFileForReading(char *filename)
{
  return openFile(filename, FILE_READ_PERMISSION);
}

FILE *openFileForWriting(char *filename)
{
  return openFile(filename, FILE_WRITE_PERMISSION);
}

FILE *openFileForReadingAndWriting(char *filename)
{
  return openFile(filename, READ_AND_WRITE_PERMISSION);
}

int deleteFile(char *filename)
{
  if (remove(filename) != 0)
  {
    fprintf(stderr, "%s", DELETE_FILE_ERROR_MSG);
    return DEFAULT_ERROR_EXIT_CODE;
  }
  return 0;
}

void inspectForSocketReadError(int charsRead)
{
  if (charsRead < 0)
    fprintf(stderr, "%s", SOCKET_READ_ERROR_MSG);
}

void handleReceiveData(int connectionSocket)
{
  char textBuffer[READ_BUFFER_SIZE];
  int charsRead;

  while ((charsRead = recv(connectionSocket, textBuffer, READ_BUFFER_SIZE, 0)) > 0)
  {
    inspectForSocketReadError(charsRead);
    for (int i = 0; i < charsRead; i++)
    {
      char curr = textBuffer[i];
      if (curr != END_STRING_CHARACTER)
        fprintf(stdout, "%c", curr);
    }
    memset(textBuffer, END_STRING_CHARACTER, READ_BUFFER_SIZE);
  }
  fprintf(stdout, NEW_LINE_CHARACTER_STR);
}