#include <stdio.h>


typedef struct {
    char* data;
    unsigned long int byteSize;
    unsigned int bitSize;
} BIT_BUFFER;

typedef struct {
    char* data;
    unsigned long int size;
} BYTE_BUFFER;

#define BIT0 0
#define BIT1 1
#define START_OF_FRAME  2
#define START_OF_DATA 3

#define ON 1
#define OFF 0

/**
 * Create a new bit buffer
 *
 * @return the created buffer
 */
BIT_BUFFER createBitBuffer();

/**
 * Create a new byte buffer
 *
 * @return the created buffer
 */
BYTE_BUFFER createByteBuffer();

/**
 * Release the memory
 *
 * @param buffer the buffer to destroy
 */
void destroyBitBuffer(BIT_BUFFER buffer);

/**
 * Release the memory
 *
 * @param buffer the buffer to destroy
 */
void destroyByteBuffer(BYTE_BUFFER buffer);

/**
 * Print all the bits from a buffer
 *
 * @param buffer the buffer holding the data
 */
//void printfBitBuffer(BIT_BUFFER buffer);
void printfBitBuffer(BYTE_BUFFER buffer);

/**
 * Print a byte in binary
 * 
 * @param byte the byte to print
 */
void printfBit(unsigned char byte);

/**
 * Print all the bytes from a buffer
 *
 * @param buffer the buffer holding the data
 */
void printfByteBuffer(BYTE_BUFFER buffer);

/**
 * Push a bit in a buffer
 *
 * @param buffer the buffer holding the data
 * @param bit the bit to push
 */
//void bitPushBit(BIT_BUFFER* buffer, unsigned char bit);

/**
 * Push a byte in a buffer
 *
 * @param buffer the buffer holding the data
 * @param byt the byte to push
 */
void pushByte(BYTE_BUFFER* buffer, unsigned char byte);

/**
 * Push a word in a buffer
 *
 * @param buffer the buffer holding the data
 * @param word the word to push
 */
void pushWord(BYTE_BUFFER* buffer, unsigned short int word);

/**
 * Push some bytes in a buffer
 *
 * @param buffer the buffer holding the data
 * @param bytes the bytes to push
 * @param len the number of bytes to push
 */
void pushBytes(BYTE_BUFFER* buffer, unsigned char *byte, unsigned int len);

/**
 * Encode bits with HomeEasy encoding (1 => 10, 0 => 01)
 * 
 * @param buffer the buffuer to encode
 * 
 * @return new buffer
 * */
BYTE_BUFFER homeEasyEncode(BYTE_BUFFER *buffer);

/**
 * Decode bits with HomeEasy encoding (1 => 10, 0 => 01)
 * 
 * @param buffer the buffuer to decode
 * 
 * @return new buffer
 * */
BYTE_BUFFER homeEasyDecode(BYTE_BUFFER *buffer);

/**
 * Encode a byte according to HomeEasy
 * 
 * @param byte the byte to encode
 * 
 * @return the encoded byte
 */
unsigned short int encodeByte(unsigned char byte);

/**
 * Decode a byte according to HomeEasy
 * 
 * @param byte the byte to decode
 * 
 * @return the decoded byte
 */
unsigned char decodeByte(unsigned short int word);

/**
 * Gives the bit at a specific position
 * 
 * @param buffer the buffer to evaluate
 * @param n the bit position
 * 
 * @return the bit value
 */
unsigned int bitAt(BYTE_BUFFER buffer, unsigned long int n);

/**
 * Append a complete command according to Chacon protocole
 *
 * @param id command id (refer to your remote)
 * @param section button section ('A' | 'B'  | 'C'  | 'D'  | 'G')
 * @param nb button number(1, 2, 3, 4)
 * @param on boolean for on/off
 */
BYTE_BUFFER createHomeEasyCommand(unsigned char* id, char section, unsigned char nb, unsigned char on);

/**
 * Append a bit that will be emitted for a specific time
 *
 * @param buffer the buffer holding the data
 * @param bit the bit to push
 * @param usec time in µs
 * @param clock frequency
 */
//void appendBit(BIT_BUFFER* buffer, unsigned char bit, unsigned int usec, unsigned int freq);

/**
 * Append data according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param type data type (BIT0 | BIT1 | START_OF_FRAME | END_OF_DATA)
 * @param clock frequency
 */
//void appendData(BIT_BUFFER* buffer, unsigned int type, unsigned int freq);

/**
 * Append a byte according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param byte the byte to append
 * @param clock frequency
 */
//void appendByte(BIT_BUFFER* buffer, unsigned char byte, unsigned int freq);

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
//void pushCode(BIT_BUFFER* buffer, unsigned char* id, unsigned char section, unsigned char nb, unsigned char on, unsigned char global, unsigned int freq);
