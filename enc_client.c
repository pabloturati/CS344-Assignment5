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

int main(int argc, char *argv[])
{
  // Input data validations
  validateArgCount(argc, ENC_CLIENT_ARG_COUNT, ENC_CLIENT_ARG_COUNT_ERROR_MSG);
  int fileLength = validateTextFileAndKey(argv[1], argv[2]);

  // Create a socket, setup address struct and connect to server
  int socketFD = createClientSocketAndConnectServer(atoi(argv[3]));

  // Send plaintext and key files to server
  handleSendFilesToServer(socketFD, argv[1], argv[2]);

  // Get encrypted data from server and write it to stdout
  handleReceiveData(socketFD);

  // Close socket
  close(socketFD);
  return 0;
}