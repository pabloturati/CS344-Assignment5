#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../constants/constants.h"
#include "../sharedMethods/sharedMethods.h"
#include "../encryptionMethods/encryptionMethods.h"
#include "../serverNetworkMethods/serverNetworkMethods.h"
#include "serverDataMethods.h"

int validateHandshake(int connectionSocket, char expectedHandshake)
{
  // Validate client with a handshake
  char receiveHandshakeBuffer[2];
  int charsRead = recv(connectionSocket, receiveHandshakeBuffer, 1, 0);
  inspectForSocketReadError(charsRead);

  // Terminate the string to send it back to the client to confirm
  receiveHandshakeBuffer[1] = END_STRING_CHARACTER;
  send(connectionSocket, receiveHandshakeBuffer, 2, 0);

  //Terminate if handshake was not successful
  if (expectedHandshake != receiveHandshakeBuffer[0])
    exitWithError(SERVER_WRONG_CLIENT_CONNECTION, WRONG_CLIENT_SERVER_CONNECTION_CODE);
  return 0;
}

void verifyKeyAndFileSizesMatch(int rawTextLength, int keyLength)
{
  if (rawTextLength != keyLength)
    exitWithError(SERVER_KEY_LENGTH_ERROR_MSG, DEFAULT_ERROR_EXIT_CODE);
}

int countTextLength(FILE *fp)
{
  char c = fgetc(fp);
  int size = 0;
  while (c != EOF)
  {
    c = fgetc(fp);
    ++size;
  }
  return size;
}

void writeTempRawTextAndKeyFiles(int connectionSocket, FILE *tempKeyFp, FILE *tempRawTextFp)
{
  char textBuffer[READ_BUFFER_SIZE];
  int charsRead, signalCounter = 0, rawTextLength = 0, keyLength = 0;

  while ((charsRead = recv(connectionSocket, textBuffer, READ_BUFFER_SIZE, 0)) > 0)
  {
    inspectForSocketReadError(charsRead);

    //Write line character by character
    for (int i = 0; i < charsRead; i++)
    {
      char curr = textBuffer[i];
      // If current char is a change flag
      if (signalCounter == 1)
      {
        if (curr == FILE_CHANGE_FLAG)
        {
          ++signalCounter;
          break;
        }
        fputc(curr, tempKeyFp);
        ++keyLength;
      }
      if (signalCounter == 0)
      {
        if (curr == FILE_CHANGE_FLAG)
        {
          ++signalCounter;
          continue;
        }
        fputc(curr, tempRawTextFp);
        ++rawTextLength;
      }
    }
    if (signalCounter == 2)
      break;
    memset(textBuffer, END_STRING_CHARACTER, READ_BUFFER_SIZE);
  }

  // Put a new line character in each file
  fputc(NEW_LINE_CHARACTER, tempRawTextFp);
  fputc(NEW_LINE_CHARACTER, tempKeyFp);

  // Validates that key and raw text file lengths match
  verifyKeyAndFileSizesMatch(rawTextLength, keyLength);
}

void closeAndDeleteFile(FILE *fd, char *filename)
{
  fclose(fd);
  deleteFile(filename);
  free(filename);
}

void handleServerFileProcess(int connectionSocket, void (*encryptionFunction)(FILE *, FILE *, FILE *))
{
  // Create temporar filenames
  char *tempRawTextFilename = createDynamicFilenameWithProcessId(TEMP_RAWFILE_FILENAME_PREFIX);
  char *tempKeyFilename = createDynamicFilenameWithProcessId(TEMP_KEY_FILENAME_PREFIX);
  char *tempEncryptedTextFilame = createDynamicFilenameWithProcessId(TEMP_ENCRYPTED_FILENAME_PREFIX);

  // Open source files for writing
  FILE *tempRawTextFp = openFileForReadingAndWriting(tempRawTextFilename);
  FILE *tempKeyFp = openFileForReadingAndWriting(tempKeyFilename);

  writeTempRawTextAndKeyFiles(connectionSocket, tempKeyFp, tempRawTextFp);

  // Rewind source file descriptors
  rewind(tempRawTextFp);
  rewind(tempKeyFp);

  // Create a temporary encrypt file and wirte it
  FILE *tempEncryptedFp = openFileForReadingAndWriting(tempEncryptedTextFilame);

  // Encrypt or decrypt file (passed as function pointer)
  (*encryptionFunction)(tempRawTextFp, tempKeyFp, tempEncryptedFp);

  // Remove temporary source files
  closeAndDeleteFile(tempRawTextFp, tempRawTextFilename);
  closeAndDeleteFile(tempKeyFp, tempKeyFilename);

  // Rewind encryption file after writing it
  rewind(tempEncryptedFp);

  // Print encryption file contents to server console
  int encryptedFileSize = countTextLength(tempEncryptedFp);

  // Rewind encryption file after writing it
  rewind(tempEncryptedFp);

  sendFileUsingSocket(connectionSocket, tempEncryptedFp, encryptedFileSize);

  // Remove temporary encryption output file
  closeAndDeleteFile(tempEncryptedFp, tempEncryptedTextFilame);
}

void transformData(
    FILE *tempRawTextFp,
    FILE *tempKeyFp,
    FILE *tempEncryptedTextFd,
    char (*cryptographicMethod)(char, char))
{
  char rawTextChar, keyChar, encryptedChar;
  while (TRUE)
  {
    rawTextChar = fgetc(tempRawTextFp);
    keyChar = fgetc(tempKeyFp);
    if (rawTextChar == NEW_LINE_CHARACTER || rawTextChar == EOF)
      break;
    encryptedChar = cryptographicMethod(rawTextChar, keyChar);
    fputc(encryptedChar, tempEncryptedTextFd);
  }
  fputc(NEW_LINE_CHARACTER, tempEncryptedTextFd);
}

void sendFileUsingSocket(int connectionSocket, FILE *filePointer, const int fileSize)
{
  //Read char by char to buffer
  char buffer[WRITE_BUFFER_SIZE];
  memset(buffer, END_STRING_CHARACTER, WRITE_BUFFER_SIZE); //FIX is this needed?
  int transmissionCounter = 0;
  const int adjustedFilesize = fileSize - 1;

  while (transmissionCounter < adjustedFilesize)
  {
    int buffIdx = 0;
    // Read char by char into the buffer
    while (buffIdx < WRITE_BUFFER_SIZE - 1)
    {
      char curr = fgetc(filePointer);
      if (curr == NEW_LINE_CHARACTER || curr == EOF)
        break;
      buffer[buffIdx] = curr;
      ++buffIdx;
    }
    //Terminate package with null character
    buffer[buffIdx + 1] = END_STRING_CHARACTER;

    // Send data to client
    int charsRead = send(connectionSocket, buffer, buffIdx + 1, 0);
    if (charsRead < 0)
      fprintf(stderr, "%s", SERVER_ERROR_WRITING_TO_SOCKET_MSG);

    //Increase transmission counter
    transmissionCounter += buffIdx;

    //Clear buffer
    memset(buffer, END_STRING_CHARACTER, WRITE_BUFFER_SIZE);
  }
}
