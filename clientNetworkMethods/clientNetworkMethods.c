#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include "../sharedMethods/sharedMethods.h"
#include "../constants/constants.h"

// Set up the address struct for the client socket
void setupClientAddressStruct(struct sockaddr_in *address, int portNumber, char *hostname)
{
  // Clear out the address struct
  memset((char *)address, END_STRING_CHARACTER, sizeof(*address));

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  struct hostent *hostInfo = gethostbyname(hostname);
  if (hostInfo == NULL)
  {
    fprintf(stderr, "%s", CLIENT_NO_HOST_ERROR_MSG);
    exit(0);
  }
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char *)&address->sin_addr.s_addr,
         hostInfo->h_addr_list[0],
         hostInfo->h_length);
}

int createClientSocketAndConnectServer(int portNumber)
{
  struct sockaddr_in serverAddress;
  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD < 0)
    exitWithError(CLIENT_ERROR_OPENING_SOCKER_MSG, DEFAULT_ERROR_EXIT_CODE);

  // Set up the server address struct
  setupClientAddressStruct(&serverAddress, portNumber, HOSTNAME);

  // Connect to server
  if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    exitWithError(CLIENT_ERROR_CONNECTING_MSG, DEFAULT_ERROR_EXIT_CODE);

  return socketFD;
}