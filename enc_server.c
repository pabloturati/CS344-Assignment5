#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "sharedMethods/sharedMethods.h"
#include "constants/constants.h"
#include "encryptionMethods/encryptionMethods.h"
#include "serverNetworkMethods/serverNetworkMethods.h"
#include "serverDataMethods/serverDataMethods.h"

char *ENC_SERVER_ARG_COUNT_ERROR_MSG = "ENC_SERVER_ERROR: %s port\n";

int main(int argc, char *argv[])
{
  int connectionSocket;

  // Check usage & args
  validateArgCount(argc, ENC_SERVER_ARG_COUNT, ENC_SERVER_ARG_COUNT_ERROR_MSG);

  // Create the socket that will listen for connections
  int listenSocket = createServerSocketAndListenForConnections(atoi(argv[1]));

  // Accept a connection, blocking if one is not available until one connects
  while (TRUE)
  {
    // Accept the connection request which creates a connection socket
    connectionSocket = acceptClientConnection(listenSocket);

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
    writeEncryptedFile(tempRawTextFp, tempKeyFp, tempEncryptedFp);

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
    close(connectionSocket);
  }
  // Close the listening socket
  close(listenSocket);
  return 0;
}
