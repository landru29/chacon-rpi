#include <stdio.h>


typedef struct {
    unsigned long int byteSize;
    unsigned int bitSize;
    char* data;
} BUFFER;

#define BIT0 0
#define BIT1 1
#define START_OF_FRAME  2
#define START_OF_DATA 3

#define ON 1
#define OFF 0

#define NUMBER1 0
#define NUMBER2 1
#define NUMBER3 2
#define NUMBER4 3

#define SECTION_A 0
#define SECTION_B 1
#define SECTION_C 2
#define SECTION_D 3

#define GLOBAL 1
#define NO_GLOBAL 0

/**
 * Create a new buffer
 *
 * @return the created buffer
 */
BUFFER createBuffer();

/**
 * Release the memory
 *
 * @param buffer the buffer to destroy
 */
void destroyBuffer(BUFFER buffer);

/**
 * Print all the bits from a buffer
 *
 * @param buffer the buffer holding the data
 */
void printfBinaryBuffer(BUFFER buffer);

/**
 * Push a bit in a buffer
 *
 * @param buffer the buffer holding the data
 * @param bit the bit to push
 */
void pushBit(BUFFER* buffer, unsigned char bit);

/**
 * Append a bit that will be emitted for a specific time
 *
 * @param buffer the buffer holding the data
 * @param bit the bit to push
 * @param usec time in µs
 * @param clock frequency
 */
void appendBit(BUFFER* buffer, unsigned char bit, unsigned int usec, unsigned int freq);

/**
 * Append data according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param type data type (BIT0 | BIT1 | START_OF_FRAME | END_OF_DATA)
 * @param clock frequency
 */
void appendData(BUFFER* buffer, unsigned int type, unsigned int freq);

/**
 * Append a byte according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param byte the byte to append
 * @param clock frequency
 */
void appendByte(BUFFER* buffer, unsigned char byte, unsigned int freq);

/**
 * Append a complete command according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param id command id (refer to your remote)
 * @param section button section (0:A, 1:B, 2:C, 3:D)
 * @param nb button number(1, 2, 3, 4)
 * @param on boolean for on/off
 * @param global if true G button is selected (nb will be ignore)
 * @param clock frequency
 */
void pushCode(BUFFER* buffer, unsigned char* id, unsigned char section, unsigned char nb, unsigned char on, unsigned char global, unsigned int freq);
