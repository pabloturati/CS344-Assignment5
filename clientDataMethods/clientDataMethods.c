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
int sendFileToServer(char *filename, int socketFD, int specificSize)
{
  FILE *fp = openFileForReading(filename);
  char *line = NULL;
  size_t len = 0;
  ssize_t readSize;
  int size;

  // Write plaintext file to the server
  while ((readSize = getline(&line, &len, fp)) != -1)
  {
    // Use size passed as param. Else calculate it based on endline character.
    size = specificSize == 0
               ? strcspn(line, NEW_LINE_CHARACTER_STR)
               : specificSize;

    line[size] = END_STRING_CHARACTER;
    int charsWritten = send(socketFD, line, strlen(line), 0);
    if (charsWritten < 0)
    {
      freeMemoryAndCloseFile(line, fp);
      exitWithError(CLIENT_WRITE_TO_SOCKET_ERROR_MSG, 1);
    }
    if (charsWritten < strlen(line))
    {
      freeMemoryAndCloseFile(line, fp);
      exitWithError(CLIENT_INCOMPLETE_WRITE_TO_SOCKET_WARNING, 1);
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
  FILE *fp = openFileForReading(filename);
  char *line = NULL;
  size_t len = 0;
  ssize_t readSize;
  int size;

  // Write plaintext file to the server
  while ((readSize = getline(&line, &len, fp)) != -1)
  {
    // Calculates the length of line (regardless of it containing new line)
    size = strcspn(line, NEW_LINE_CHARACTER_STR);

    for (int i = 0; i < size; i++)
    {
      if (!isValidCharacter(line[i]))
      {
        freeMemoryAndCloseFile(line, fp);
        return -1;
      }
    }
    freeMemoryAndCloseFile(line, fp);
  }
  return size;
}

int validateTextFileAndKey(char *textFilename, char *keyFilename)
{
  int textfileLength = validateCharacters(textFilename);
  if (textfileLength < 0)
    exitWithError(CLIENT_INVALID_CHARACTERS_MSG, 1);
  int keyFileLength = validateCharacters(keyFilename);
  if (textfileLength > keyFileLength)
    exitWithDinamicallyGeneratedMessage(KEY_FILE_SIZE_ERROR_MSG, keyFilename);
  return textfileLength;
}

void handleSendFilesToServer(int socketFD, char *file1, char *file2, int (*handshakeHandler)(int))
{
  // Send handshake
  handshakeHandler(socketFD);

  // Send first file to server
  int textSendLength = sendFileToServer(file1, socketFD, 0);

  // Send flag to mark end of first file
  send(socketFD, FILE_CHANGE_FLAG_STR, 1, 0);

  // Send key file to server
  sendFileToServer(file2, socketFD, textSendLength);

  // Send flag to mark end of second file
  send(socketFD, FILE_CHANGE_FLAG_STR, 1, 0);
}