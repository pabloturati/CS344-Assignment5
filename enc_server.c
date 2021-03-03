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

int main(int argc, char *argv[])
{
  int connectionSocket, charsRead;
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Check usage & args
  if (argc < 2)
  {
    exitWithError("ENC_SERVER_ERROR: %s port\n", 1);
  }

  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0)
  {
    exitWithError("ENC_SERVER_ERROR opening socket", 1);
  }

  // Set up the address struct for the server socket
  setupServerAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket,
           (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0)
  {
    exitWithError("ENC_SERVER_ERROR on binding", 1);
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5);

  // Accept a connection, blocking if one is not available until one connects
  while (1)
  {
    // Accept the connection request which creates a connection socket
    connectionSocket = accept(listenSocket,
                              (struct sockaddr *)&clientAddress,
                              &sizeOfClientInfo);
    if (connectionSocket < 0)
    {
      fprintf(stderr, "ENC_SERVER_ERROR on accept\n");
    }

    printf("ENC_SERVER: Connected to client running at host %d port %d\n",
           ntohs(clientAddress.sin_addr.s_addr),
           ntohs(clientAddress.sin_port));

    // Read the plain text
    char plainTextBuffer[256];
    memset(plainTextBuffer, '\0', 256);

    charsRead = recv(connectionSocket, plainTextBuffer, 255, 0);
    if (charsRead < 0)
    {
      fprintf(stderr, "ENC_SERVER_ERROR reading from socket\n");
    }
    printf("ENC_SERVER: I received this plain text from the client: \"%s\"\n", plainTextBuffer);

    // Read the key
    char keyBuffer[256];
    memset(keyBuffer, '\0', 256);
    charsRead = recv(connectionSocket, keyBuffer, 255, 0);
    if (charsRead < 0)
    {
      fprintf(stderr, "ENC_SERVER_ERROR reading from socket\n");
    }
    printf("ENC_SERVER: I received this key from the client: \"%s\"\n", keyBuffer);

    char *encryptedMessage = encryptionHandler(plainTextBuffer, keyBuffer, charsRead);
    printf("ENC_SERVER: Encrypted is %s with size %d\n", encryptedMessage, charsRead);

    charsRead = send(connectionSocket,
                     encryptedMessage, charsRead, 0);
    if (charsRead < 0)
    {
      fprintf(stderr, "ENC_SERVER_ERROR writing to socket\n");
    }
    // Close the connection socket for this client
    close(connectionSocket);
  }
  // Close the listening socket
  close(listenSocket);
  return 0;
}
