#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include "emitlib.h"


unsigned int timings[4][2] = {
    {250, 250},  //  bit 0
    {250, 1200}, //  bit 1
    {250, 2600}, //  start of frame
    {250, 9900}  //  end of data
};

unsigned char numbering[4] = {0x05, 0x06, 0x09, 0x0A};

/**
 * Create a new buffer
 *
 * @return the created buffer
 */
BIT_BUFFER createBitBuffer()
{
    BIT_BUFFER buffer;
    buffer.byteSize = 1;
    buffer.bitSize = 0;
    buffer.data = (char*) malloc(1);
    buffer.data[0] = 0;
    return buffer;
}

/**
 * Release the memory
 *
 * @param buffer the buffer to destroy
 */
void destroyBitBuffer(BIT_BUFFER buffer)
{
    free(buffer.data);
}

/**
 * Create a new byte buffer
 *
 * @return the created buffer
 */
BIT_BUFFER createByteBuffer()
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
void destroyByteBuffer(BIT_BUFFER buffer)
{
    free(buffer.data);
}

/**
 * Print all the bits from a buffer
 *
 * @param buffer the buffer holding the data
 */
void printfBitBuffer(BIT_BUFFER buffer)
{
    printf("bytes: %d\nbits: %d\n", buffer.byteSize, buffer.bitSize);
    unsigned char byte;
    unsigned char bit;
    unsigned int x;
    int i;
    for(x=0; x<buffer.byteSize; x++) {
        byte = buffer.data[x];
        for(i=0x80; i>0; i>>=1) {
            if ((bit==0) && (byte & i)) {
                fprintf(stdout, "\n");
            }
            bit = ((byte & i) == i);
            fprintf(stdout, "%d", bit);
        }
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
        fprintf(stdout, "%02X ", buffer.data[i];
    }
}

/**
 * Push a bit in a buffer
 *
 * @param buffer the buffer holding the data
 * @param bit the bit to push
 */
void pushBit(BIT_BUFFER* buffer, unsigned char bit)
{
    buffer->data[buffer->byteSize-1] |= bit << (7-buffer->bitSize);
    buffer->bitSize++;
    if (buffer->bitSize == 8) {
        buffer->bitSize = 0;
        buffer->byteSize++;
        buffer->data = (char*)realloc(buffer->data, buffer->byteSize);
        buffer->data[buffer->byteSize-1] = 0;
    }
}

/**
 * Push a byte in a buffer
 *
 * @param buffer the buffer holding the data
 * @param byte the byte to push
 */
void pushByte(BYTE_BUFFER* buffer, unsigned char byte)
{
    buffer->size++
    buffer->data = (char*)realloc(buffer->size);
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
    unsigned char* data = &word;
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
    buffer->data = (char*)realloc(buffer->size+len);
    memcpy(&buffer->data[buffer->size], byte, len);
    buffer->size += len;
}

/**
 * Encode bits with HomeEasy encoding (1 => 10, 0 => 01)
 * 
 * @param buffer the buffuer to encode
 * 
 * @return new buffer
 * */
BYTE_BUFFER homeEasyEncode(BYTE_BUFFER *buffer)
{
    BYTE_BUFFER result = createByteBuffer();
    unsigned char byte;
    unsigned char encodedByte;
    unsigned int i;
    for(i=0; i<buffer->size; i++) {
        byte = buffer->data[i];
        pushWord(buffer, encodeByte(byte);
    }
    return result;
}

/**
 * Decode bits with HomeEasy encoding (1 => 10, 0 => 01)
 * 
 * @param buffer the buffuer to decode
 * 
 * @return new buffer
 * */
BYTE_BUFFER homeEasyDecode(BYTE_BUFFER *buffer)
{
    BYTE_BUFFER result = createByteBuffer();
    unsigned short int word;
    unsigned char *byte = &word;
    unsigned int i;
    for(i=0; i<(buffer->size+1)/2; i++) {
        byte[1] = buffer->data[2*i];
        byte[0] = (2*i+1 < buffer->size ? buffer->data[2*i+1]:0);
        pushByte(buffer, decodeByte(word));
    }
    return result;
}

/**
 * Decode a byte according to HomeEasy
 * 
 * @param byte the byte to decode
 * 
 * @return the decoded byte
 */
unsigned char decodeByte(unsigned short int byte)
{
    unsigned char decodedChar=0;
    int j;
    int shift = 6;
    for(j=0x8000;j>0; j>>=2) {
      decodedChar |= (((byte & j) == j) ? 0x01 : 0x00) << shift;
      shift -=1;
    }
    return decodedChar;
}

/**
 * Encode a byte according to HomeEasy
 * 
 * @param byte the byte to encode
 * 
 * @return the encoded byte
 */
unsigned short int encodeByte(unsigned char byte)
{
    unsigned short int encodedChar=0;
    int j;
    int shift = 6;
    for(j=0x80;j>0; j>>=1) {
      encodedChar |= (((byte & j) == j) ? 0x02 : 0x01) << shift;
      shift -=2;
    }
    return encodedChar;
}

/**
 * Append a bit that will be emitted for a specific time
 *
 * @param buffer the buffer holding the data
 * @param bit the bit to push
 * @param usec time in µs
 * @param clock frequency
 */
void appendBit(BIT_BUFFER* buffer, unsigned char bit, unsigned int usec, unsigned int freq)
{
    unsigned int i;
    for(i=0; i<(usec * freq) / 1e6; i++) {
        pushBit(buffer, bit);
    }
}

/**
 * Append data according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param type data type (BIT0 | BIT1 | START_OF_FRAME | END_OF_DATA)
 * @param clock frequency
 */
void appendData(BIT_BUFFER* buffer, unsigned int type, unsigned int freq)
{
    appendBit(buffer, 1, timings[type][0], freq);
    appendBit(buffer, 0, timings[type][1], freq);
}

/**
 * Append a byte according to Chacon protocole
 *
 * @param buffer the buffer holding the data
 * @param byte the byte to append
 * @param clock frequency
 */
void appendByte(BIT_BUFFER* buffer, unsigned char byte, unsigned int freq)
{
    int i;
    for(i=0x80; i>0; i>>=1) {
        appendData(buffer, ((byte & i) == i), freq);
    }
    printf("%02X ", byte);
}


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
void pushCode(BIT_BUFFER* buffer, unsigned char* id, unsigned char section, unsigned char nb, unsigned char on, unsigned char global, unsigned int freq)
{
    unsigned char byte6 = (id[6] & 0xf0) + numbering[on + (global ? 2 : 0)];
    unsigned char byte7 = (numbering[section] << 4) + numbering[nb];
    unsigned int i;
    if (global) {
        byte7 = 0x55;
    }
    appendData(buffer, START_OF_FRAME, freq);
    printf("SOF ");
    for(i=0; i<6; i++) {
            appendByte(buffer, id[i], freq);
    }
    appendByte(buffer, byte6, freq);
    appendByte(buffer, byte7, freq);
    appendData(buffer, END_OF_DATA, freq);
    printf("EOD ");
}
