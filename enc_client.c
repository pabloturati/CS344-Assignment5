#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "sharedMethods/sharedMethods.h"
#include "constants/constants.h"
#include "encryptionMethods/encryptionMethods.h"
#include "clientDataMethods/clientDataMethods.h"
#include "clientNetworkMethods/clientNetworkMethods.h"

int sendEncryptionHandshakeMarker(int socketFD);

int main(int argc, char *argv[])
{
  // Input data validations
  validateArgCount(argc, ENC_CLIENT_ARG_COUNT, ENC_CLIENT_ARG_COUNT_ERROR_MSG);
  int fileLength = validateTextFileAndKey(argv[1], argv[2]);

  // Create a socket, setup address struct and connect to server
  int socketFD = createClientSocketAndConnectServer(atoi(argv[3]));

  // Send plaintext and key files to server
  handleSendFilesToServer(socketFD, argv[1], argv[2], sendEncryptionHandshakeMarker);

  // Get encrypted data from server and write it to stdout
  handleReceiveData(socketFD);

  // Close socket
  close(socketFD);
  return 0;
}

int sendEncryptionHandshakeMarker(int socketFD)
{
  send(socketFD, ENC_HANDSHAKE_MARKER, 1, 0);
  char receiveHandshakeBuffer[2];
  int charsRead = recv(socketFD, receiveHandshakeBuffer, 1, 0);
  inspectForSocketReadError(charsRead);
  if (ENC_HANDSHAKE_MARKER_CHAR != receiveHandshakeBuffer[0])
    exitWithError(CLIENT_WRONG_SERVER_CONNECTION, WRONG_CLIENT_SERVER_CONNECTION_CODE);
  return 0;
}