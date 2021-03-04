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
#define BUFFER_SIZE 2000

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

int main(int argc, char *argv[])
{
  struct sockaddr_in serverAddress;
  char buffer[BUFFER_SIZE];

  // Check usage & args
  if (argc < 3)
  {
    exitWithError("USAGE: %s plaintextFilepath keyFilepath portNumber", 0);
  }

  // Validate characters
  validateCharacters(argv[1]);

  // Create a socket, setup address struct and connect to server
  int socketFD = createSocketAndConnectServer(atoi(argv[3]));

  // Send plaintext file to server
  int plainTextSize = sendFileToServer(argv[1], socketFD);

  // Send key file to server
  int keyTextSize = sendFileToServer(argv[2], socketFD);

  if (plainTextSize != keyTextSize)
  {
    exitWithError("CLIENT: Plain text file length does not match key length", 1);
  }

  // Get return message from server
  // Clear out the buffer again for reuse
  memset(buffer, '\0', sizeof(buffer));
  // Read data from the socket, leaving \0 at end
  int charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
  if (charsRead < 0)
  {
    exitWithError("CLIENT: ERROR reading from socket", 1);
  }
  fprintf(stdout, "%s\n", buffer);

  // Close the socket
  close(socketFD);
  return 0;
}