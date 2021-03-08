#include <string.h>
#include "constants.h"

/****** Error messages *******/
const char *KEY_GEN_TOO_FEW_ARGS = "Please include key lenght as integer greater then 0";

// ENC_CLIENT
char *ENC_CLIENT_ARG_COUNT_ERROR_MSG = "ENC_CLIENT: Usage = %s plaintextFilename keyFilename portNumber";
char *DEC_CLIENT_ARG_COUNT_ERROR_MSG = "ENC_CLIENT: Usage = %s decryptedTextFilepath keyFilepath portNumber";

char *SERVER_ARG_COUNT_ERROR_MSG = "SERVER_ERROR: %s port\n";
