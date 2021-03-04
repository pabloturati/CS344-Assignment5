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
  memset((char *)address, '\0', sizeof(*address));

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  struct hostent *hostInfo = gethostbyname(hostname);
  if (hostInfo == NULL)
  {
    fprintf(stderr, "CLIENT: ERROR, no such host\n");
    exit(0);
  }
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char *)&address->sin_addr.s_addr,
         hostInfo->h_addr_list[0],
         hostInfo->h_length);
}

// int createSocketAndConnectServer(int portNumber)
// {
//   struct sockaddr_in serverAddress;
//   int socketFD = socket(AF_INET, SOCK_STREAM, 0);
//   if (socketFD < 0)
//   {
//     exitWithError("CLIENT: ERROR opening socket", 1);
//   }

//   // Set up the server address struct
//   setupClientAddressStruct(&serverAddress, portNumber, HOSTNAME);

//   // Connect to server
//   if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
//   {
//     exitWithError("CLIENT: ERROR connecting", 1);
//   }
//   return socketFD;
// }