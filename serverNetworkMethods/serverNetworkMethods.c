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

// Set up the address struct for the server socket
void setupServerAddressStruct(struct sockaddr_in *address, int portNumber)
{
  // Clear out the address struct
  memset((char *)address, '\0', sizeof(*address));

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

int createServerSocketAndListenForConnections(int portNumber)
{
  struct sockaddr_in serverAddress;

  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0)
  {
    exitWithError("ENC_SERVER_ERROR opening socket", 1);
  }

  // Set up the address struct for the server socket
  setupServerAddressStruct(&serverAddress, portNumber);

  // Associate the socket to the port
  if (bind(listenSocket,
           (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0)
  {
    exitWithError("ENC_SERVER_ERROR on binding", 1);
  }

  // Start listening for connetions. Allow up to a max number of connections to queue up
  listen(listenSocket, MAX_NUM_OF_CONNECTIONS);
  return listenSocket;
}

int acceptClientConnection(int listenSocket)
{
  struct sockaddr_in clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Accept the connection request which creates a connection socket
  int connectionSocket = accept(listenSocket,
                                (struct sockaddr *)&clientAddress,
                                &sizeOfClientInfo);
  if (connectionSocket < 0)
  {
    fprintf(stderr, "ENC_SERVER_ERROR on accept\n");
  }

  printf("ENC_SERVER: Connected to client running at host %d port %d\n",
         ntohs(clientAddress.sin_addr.s_addr),
         ntohs(clientAddress.sin_port));
  return connectionSocket;
}