#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include "../sharedMethods/sharedMethods.h"
#include "../encryptionMethods/encryptionMethods.h"
#include "../constants/constants.h"

/*
Function to open a file for reading and sending a file using a socket
Input: fileName-> path to file to send (string), 
  socketFd-> socket file descriptor (integer) 
Output: returns encripted message of the same length as key/message
*/
int sendFileToServer(char *fileName, int socketFD)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t readSize;
  int size;

  fp = fopen(fileName, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  // Write plaintext file to the server
  while ((readSize = getline(&line, &len, fp)) != -1)
  {
    size = strcspn(line, "\n");
    line[size] = '\0';
    int charsWritten = send(socketFD, line, strlen(line), 0);
    if (charsWritten < 0)
    {
      freeMemoryAndCloseFile(line, fp);
      exitWithError("CLIENT: ERROR writing to socket", 1);
    }
    if (charsWritten < strlen(line))
    {
      freeMemoryAndCloseFile(line, fp);
      exitWithError("CLIENT: WARNING: Not all data written to socket!\n", 1);
    }
  }
  freeMemoryAndCloseFile(line, fp);
  return size;
}

/*
Function to validate that all characters in the source file are valid
Input: filename-> path to file to send (string), 
  socketFd-> socket file descriptor (integer) 
Output: returns encripted message of the same length as key/message
*/
int validateCharacters(char *filename)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t readSize;
  int size;

  fp = fopen(filename, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  // Write plaintext file to the server
  while ((readSize = getline(&line, &len, fp)) != -1)
  {
    // Calculates the length of line (regardless of it containing new line)
    size = strcspn(line, "\n");

    for (int i; i < size; i++)
    {
      if (!isValidCharacter(line[i]))
      {
        freeMemoryAndCloseFile(line, fp);
        exitWithError("CLIENT: ERROR source file contains invalid characters", 1);
      }
    }
    freeMemoryAndCloseFile(line, fp);
  }
  return size;
}