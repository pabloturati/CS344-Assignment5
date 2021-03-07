#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
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

void validateArgCount(int argc, int expectedArgc, char *errorMsg)
{
  if (argc < expectedArgc)
    exitWithError(errorMsg, 1);
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
    exitWithError("Error opening file\n", EXIT_FAILURE);
  return fd;
}

FILE *openFileForReading(char *filename)
{
  return openFile(filename, "r");
}

FILE *openFileForWriting(char *filename)
{
  return openFile(filename, "w");
}

FILE *openFileForReadingAndWriting(char *filename)
{
  return openFile(filename, "w+");
}

void sendFileUsingSocket(int connectionSocket, FILE *filePointer, const int fileSize)
{
  //Read char by char to buffer
  char buffer[WRITE_BUFFER_SIZE];
  memset(buffer, '\0', WRITE_BUFFER_SIZE); //FIX is this needed?
  int transmissionCounter = 0;
  const int adjustedFilesize = fileSize - 1;

  while (transmissionCounter < adjustedFilesize)
  {
    int buffIdx = 0;
    // Read char by char into the buffer
    while (buffIdx < WRITE_BUFFER_SIZE - 1)
    {
      char curr = fgetc(filePointer);
      if (curr == '\n' || curr == EOF)
        break;
      buffer[buffIdx] = curr;
      ++buffIdx;
    }
    //Terminate package with null character
    buffer[buffIdx + 1] = '\0';

    // Send data to client
    int charsRead = send(connectionSocket, buffer, buffIdx + 1, 0);
    if (charsRead < 0)
    {
      fprintf(stderr, "ENC_SERVER_ERROR writing to socket\n");
    }

    //Increase transmission counter
    transmissionCounter += buffIdx;

    //Clear buffer
    memset(buffer, '\0', WRITE_BUFFER_SIZE);
  }
}

int deleteFile(char *filename)
{
  if (remove(filename) != 0)
    printf("Unable to delete the file");
  return 0;
}

void inspectForSocketReadError(int charsRead)
{
  if (charsRead < 0)
  {
    fprintf(stderr, "ERROR: reading from socket\n");
  }
}

void handleReceiveData(int connectionSocket)
{
  char textBuffer[READ_BUFFER_SIZE];
  int charsRead;

  // char stdOutWriteBuffer[3];

  while ((charsRead = recv(connectionSocket, textBuffer, READ_BUFFER_SIZE, 0)) > 0)
  {
    inspectForSocketReadError(charsRead);
    for (int i = 0; i < charsRead; i++)
    {
      char curr = textBuffer[i];
      if (curr != '\0')
        fprintf(stdout, "%c", curr);
    }
    memset(textBuffer, '\0', READ_BUFFER_SIZE);
  }
  fprintf(stdout, "\n");
}