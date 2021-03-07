#ifndef CONSTANTS_H
#define CONSTANTS_H
#define UPPER_RAND_RANGE 26
#define LOWER_RAND_RANGE 0
#define ASCII_VAL_OF_A 65
#define ASCII_VAL_OF_Z 90
#define ASVCII_VAL_OF_SPACE 32
#define SPACE_CHAR ' '
#define SPACE_RAND_VAL 26
#define TOTAL_AMOUNT_OF_CHARS 27
#define HOSTNAME "localhost"
#define FALSE 0
#define TRUE 1
#define MAX_NUM_OF_CONNECTIONS 5

#define TEMP_FILENAME_SIZE 128
#define TEMP_RAWFILE_FILENAME_PREFIX "tempReceivedRawTextFile_%d"
#define TEMP_KEY_FILENAME_PREFIX "tempReceivedKeyFile_%d"
#define TEMP_ENCRYPTED_FILENAME_PREFIX "tempEncryptedFile_%d"
#define FILE_CHANGE_FLAG '&'
#define FILE_CHANGE_FLAG_STR "&"
#define NEW_LINE_CHARACTER '\n'
#define NEW_LINE_CHARACTER_STR "\n"
#define END_STRING_CHARACTER '\0'

#define READ_BUFFER_SIZE 4
#define WRITE_BUFFER_SIZE 4

#define ENC_SERVER_ARG_COUNT 2
#define ENC_CLIENT_ARG_COUNT 3

/****** Error messages *******/
const char *KEY_GEN_TOO_FEW_ARGS;

#endif