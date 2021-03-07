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

void verifyKeyAndFileSizesMatch(int rawTextLength, int keyLength)
{
  if (rawTextLength != keyLength)
    exitWithError("ENC_SERVER_ERROR: Raw text file length does not match key", 1);
}

void writeEncryptedFile(FILE *tempRawTextFp, FILE *tempKeyFp, FILE *tempEncryptedTextFd)
{
  // printf("marcador B\n");
  char rawTextChar, keyChar, encryptedChar;
  while (TRUE)
  {
    // printf("marcador C\n");
    rawTextChar = fgetc(tempRawTextFp);
    // printf("rawTextChar: %c\n", rawTextChar);
    keyChar = fgetc(tempKeyFp);
    if (rawTextChar == '\n' || rawTextChar == EOF)
      break;
    encryptedChar = encryptChar(rawTextChar, keyChar);
    // printf("En encryption-encrypted Char: %c\n", encryptedChar);
    char result = fputc(encryptedChar, tempEncryptedTextFd);
  }
  fputc('\n', tempEncryptedTextFd);
}

int printFileContents(FILE *fp)
{
  char c = fgetc(fp);
  int size = 0;
  while (c != EOF)
  {
    printf("%c", c);
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
    memset(textBuffer, '\0', READ_BUFFER_SIZE);
  }

  // Put a new line character in each file
  fputc('\n', tempRawTextFp);
  fputc('\n', tempKeyFp);

  // Validates that key and raw text file lengths match
  verifyKeyAndFileSizesMatch(rawTextLength, keyLength);
}

void closeAndDeleteFile(FILE *fd, char *filename)
{
  fclose(fd);
  deleteFile(filename);
  free(filename);
}