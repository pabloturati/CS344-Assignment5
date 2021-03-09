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

void writeEncryptedFile(FILE *, FILE *, FILE *);

int main(int argc, char *argv[])
{
  int connectionSocket;

  // Check usage & args
  validateArgCount(argc, ENC_SERVER_ARG_COUNT, SERVER_ARG_COUNT_ERROR_MSG);

  // Create the socket that will listen for connections
  int listenSocket = createServerSocketAndListenForConnections(atoi(argv[1]));

  // Accept a connection, blocking if one is not available until one connects
  while (TRUE)
  {
    // Accept the connection request which creates a connection socket
    connectionSocket = acceptClientConnection(listenSocket);

    // Filters unwanted connections
    validateHandshake(connectionSocket, ENC_HANDSHAKE_MARKER_CHAR);

    // Encrypt file and send to client
    handleServerFileProcess(connectionSocket, writeEncryptedFile);

    close(connectionSocket);
  }
  // Close the listening socket
  close(listenSocket);
  return 0;
}

void writeEncryptedFile(FILE *tempRawTextFp, FILE *tempKeyFp, FILE *tempEncryptedTextFd)
{
  transformData(tempRawTextFp, tempKeyFp, tempEncryptedTextFd, encryptChar);
}