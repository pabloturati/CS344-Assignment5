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
  int encryptedFileSize = printFileContents(tempEncryptedFp);

  // Rewind encryption file after writing it
  rewind(tempEncryptedFp);

  sendFileUsingSocket(connectionSocket, tempEncryptedFp, encryptedFileSize);

  // Remove temporary encryption output file
  closeAndDeleteFile(tempEncryptedFp, tempEncryptedTextFilame);
}