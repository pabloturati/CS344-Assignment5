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

// FIX
#define BUFFER_SIZE 10000

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

char *ARG_COUNT_ERROR_MSG = "ENC_CLIENT: Usage = %s plaintextFilename keyFilename portNumber";

int main(int argc, char *argv[])
{
  // Validations
  validateArgCount(argc, 3, ARG_COUNT_ERROR_MSG);
  validateTextFileAndKey(argv[1], argv[2]);

  // Create a socket, setup address struct and connect to server
  int socketFD = createSocketAndConnectServer(atoi(argv[3]));

  // Send plaintext file to server
  sendFileToServer(argv[1], socketFD);

  // Send key file to server
  sendFileToServer(argv[2], socketFD);

  // Get return message from server

  // Clear out the buffer again for reuse
  // memset(buffer, '\0', sizeof(buffer));
  // Read data from the socket, leaving \0 at end

  int count = 0, total = 0;
  char buffer[BUFFER_SIZE];
  memset(buffer, '\0', sizeof(buffer));

  while ((count = recv(socketFD, &buffer[total], sizeof(buffer) - total, 0)) > 0)
  {
    // buffer[total + count + 1] = '\0';
    fprintf(stdout, "%s", &buffer[total]);
    fprintf(stdout, "");
    total += count;
  }
  fprintf(stdout, "\n");
  //  = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
  if (count < 0)
  {
    exitWithError("CLIENT: ERROR reading from socket", 1);
  }

  // int charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
  // if (charsRead < 0)
  // {
  //   exitWithError("CLIENT: ERROR reading from socket", 1);
  // }
  // fprintf(stdout, "%s\n", buffer);

  // Close the socket
  close(socketFD);
  return 0;
}