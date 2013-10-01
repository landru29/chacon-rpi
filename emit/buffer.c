#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include "buffer.h"

/**
 * Create a new byte buffer
 *
 * @return the created buffer
 */
BYTE_BUFFER createByteBuffer()
{
    BYTE_BUFFER buffer;
    buffer.size = 0;
    buffer.data = (char*) malloc(1);
    return buffer;
}

/**
 * Release the memory
 *
 * @param buffer the buffer to destroy
 */
void destroyByteBuffer(BYTE_BUFFER buffer)
{
    free(buffer.data);
}

/**
 * Print all the bits from a buffer
 *
 * @param buffer the buffer holding the data
 */
void printfBitBuffer(BYTE_BUFFER buffer)
{
    printf("bytes: %lu\nbits: %lu\n", buffer.size, buffer.size*8);
    unsigned int x;
    for(x=0; x<buffer.size ; x++) {
        printfBit(buffer.data[x]);
        printf("\n");
    }
}

/**
 * Print a byte in binary
 * 
 * @param byte the byte to print
 */
void printfBit(unsigned char byte)
{
    int i;
    for(i=0x80; i>0; i>>=1) {
        fprintf(stdout, "%d", ((byte & i) == i));
    }
}


/**
 * Print all the bytes from a buffer
 *
 * @param buffer the buffer holding the data
 */
void printfByteBuffer(BYTE_BUFFER buffer)
{
    unsigned int i;
    for(i=0; i<buffer.size; i++) {
        fprintf(stdout, "%02X ", (unsigned char)buffer.data[i]);
    }
    fprintf(stdout, "\n");
}

/**
 * Push a byte in a buffer
 *
 * @param buffer the buffer holding the data
 * @param byte the byte to push
 */
void pushByte(BYTE_BUFFER* buffer, unsigned char byte)
{
    buffer->size++;
    buffer->data = (char*)realloc(buffer->data, buffer->size);
    buffer->data[buffer->size-1] = byte;
}

/**
 * Push a word in a buffer
 *
 * @param buffer the buffer holding the data
 * @param word the word to push
 */
void pushWord(BYTE_BUFFER* buffer, unsigned short int word)
{
    unsigned char* data = (unsigned char*)&word;
    pushByte(buffer, data[1]);
    pushByte(buffer, data[0]);
}
/**
 * Push some bytes in a buffer
 *
 * @param buffer the buffer holding the data
 * @param bytes the bytes to push
 * @param len the number of bytes to push
 */
void pushBytes(BYTE_BUFFER* buffer, unsigned char *byte, unsigned int len)
{
    buffer->data = (char*)realloc(buffer->data, buffer->size+len);
    memcpy(&buffer->data[buffer->size], byte, len);
    buffer->size += len;
}

/**
 * Gives the bit at a specific position
 * 
 * @param buffer the buffer to evaluate
 * @param n the bit position
 * 
 * @return the bit value
 */
unsigned int bitAt(BYTE_BUFFER buffer, unsigned long int n)
{
    unsigned char byte = buffer.data[n / 8];
    return (byte & (0x80 >> (n % 8)) != 0);
}

