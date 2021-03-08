#include <string.h>
#include "constants.h"

/****** Error messages *******/
const char *KEY_GEN_TOO_FEW_ARGS = "Please include key lenght as integer greater then 0";

// Client UI
char *ENC_CLIENT_ARG_COUNT_ERROR_MSG = "CLIENT_ERROR: Usage = %s plaintextFilename keyFilename portNumber\n";
char *DEC_CLIENT_ARG_COUNT_ERROR_MSG = "CLIENT_ERROR: Usage = %s decryptedTextFilepath keyFilepath portNumber\n";
char *KEY_FILE_SIZE_ERROR_MSG = "CLIENT_ERROR: Key %s is too short\n";
char *CLIENT_WRITE_TO_SOCKET_ERROR_MSG = "CLIENT: ERROR writing to socket\n";
char *CLIENT_INCOMPLETE_WRITE_TO_SOCKET_WARNING = "CLIENT: WARNING: Not all data written to socket!\n";
char *CLIENT_INVALID_CHARACTERS_MSG = "CLIENT: ERROR source file contains invalid characters\n";
char *CLIENT_NO_HOST_ERROR_MSG = "CLIENT: ERROR, no such host\n";
char *CLIENT_ERROR_OPENING_SOCKER_MSG = "CLIENT: ERROR opening socket\n";
char *CLIENT_ERROR_CONNECTING_MSG = "CLIENT: ERROR connecting\n";

// Server UI
char *SERVER_ARG_COUNT_ERROR_MSG = "SERVER_ERROR: %s port\n";
char *SERVER_KEY_LENGTH_ERROR_MSG = "ENC_SERVER_ERROR: Raw text file length does not match key\n";

char *SERVER_OPEN_SOCKET_ERROR_MSG = "ENC_SERVER_ERROR opening socket\n";
char *SERVER_BINDING_ERROR_MSG = "ENC_SERVER_ERROR on binding\n";
char *SERVER_ACCEPT_CONNECTION_ERROR_MSG = "ENC_SERVER_ERROR on accept\n";
char *SERVER_CONNECTION_CONFIRM_MSG = "ENC_SERVER: Connected to client running at host %d port %d\n";

// Shared UI
char *ERROR_OPENING_FILE_MSG = "Error opening file\n";
char *SERVER_ERROR_WRITING_TO_SOCKET_MSG = "ENC_SERVER_ERROR writing to socket\n";
char *DELETE_FILE_ERROR_MSG = "Unable to delete the file\n";
char *SOCKET_READ_ERROR_MSG = "ERROR: reading from socket\n";
