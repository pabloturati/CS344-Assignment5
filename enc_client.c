#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include "sharedMethods/sharedMethods.h"
#include "constants/constants.h"
#include "encryptionMethods/encryptionMethods.h"
#include "clientDataMethods/clientDataMethods.h"
#include "clientNetworkMethods/clientNetworkMethods.h"

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

char *ENC_CLIENT_ARG_COUNT_ERROR_MSG = "ENC_CLIENT: Usage = %s plaintextFilename keyFilename portNumber";

int main(int argc, char *argv[])
{
  // Input data validations
  validateArgCount(argc, ENC_CLIENT_ARG_COUNT, ENC_CLIENT_ARG_COUNT_ERROR_MSG);
  int fileLength = validateTextFileAndKey(argv[1], argv[2]);

  // Create a socket, setup address struct and connect to server
  int socketFD = createClientSocketAndConnectServer(atoi(argv[3]));

  // Send plaintext file and key file to server
  handleSendFilesToServer(socketFD, argv[1], argv[2]);

  // Get encrypted data from server and write it to stdout
  handleReceiveData(socketFD);

  // Close socket
  close(socketFD);
  return 0;
}