#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include "./sharedMethods.h"
#include "../constants/constants.h"

void exitWithError(const char *msg, int errorCode)
{
  fprintf(stderr, "%s\n", msg);
  exit(errorCode);
}

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

/*
Function to open a file for reading and sending a file using a socket
Input: fileName-> path to file to send (string), 
  socketFd-> socket file descriptor (integer) 
Output: returns encripted message of the same length as key/message
*/
int sendFileToServer(char *fileName, int socketFD)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t readSize;
  int size;

  fp = fopen(fileName, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  // Write plaintext file to the server
  if ((readSize = getline(&line, &len, fp)) != -1)
  {
    printf("Retrieved line of length %zu:\n", readSize);
    printf("%s", line);
    size = strcspn(line, "\n");
    printf("Size %d\n", size);
    line[size] = '\0';
    int charsWritten = send(socketFD, line, strlen(line), 0);
    free(line);
    if (charsWritten < 0)
    {
      exitWithError("CLIENT: ERROR writing to socket", 1);
    }
    if (charsWritten < strlen(line))
    {
      exitWithError("CLIENT: WARNING: Not all data written to socket!\n", 1);
    }
  }
  return size;
}