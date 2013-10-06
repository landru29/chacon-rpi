#ifndef __BUFFER_H__
#define __BUFFER_H__


typedef struct {
    char* data;
    unsigned long int size;
} BYTE_BUFFER;


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
void destroyByteBuffer(BYTE_BUFFER buffer);

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
 * Gives the bit at a specific position
 * 
 * @param buffer the buffer to evaluate
 * @param n the bit position
 * 
 * @return the bit value
 */
unsigned int bitAt(BYTE_BUFFER buffer, unsigned long int n);


/**
 * Print all the bits from a buffer
 *
 * @param buffer the buffer holding the data
 */
//void printfBitBuffer(BIT_BUFFER buffer);
void printfBitBuffer(BYTE_BUFFER buffer);

#endif