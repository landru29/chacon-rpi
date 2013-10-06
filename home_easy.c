#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#ifdef __arm__
    #include <wiringPi.h>
#else
    #define LOW 0
    #define HIGH 1
    #define INPUT 0
    #define OUTPUT 1
    int wiringPiSetup(void) {return 0;}
    void pinMode (int a, int b) {a=b;}
    void delayMicroseconds(unsigned int a) {a=a;}
    void digitalWrite(int a, int b) {a=b;}
    int digitalRead(int a) {return a;}
#endif

#include "home_easy.h"
#include "buffer.h"
#include "utils.h"

unsigned int timings[5][2] = {
    //{310, 310},  // bit 0
    //{310, 1340}, // bit 1
    {275, 275},  //  bit 0
    {275, 1300}, //  bit 1
    {275, 9900},  //  start of data
    {275, 2675}, //  start of frame
    {275, 2675},  //  end of data
};

unsigned char homeEasyPinOut = 0;
unsigned char homeEasyPinIn = 2;


/**
 * Encode bits with HomeEasy encoding (1 => 10, 0 => 01)
 *
 * @param frame 32-bit frame to encode
 *
 * @return new buffer
 * */
BYTE_BUFFER homeEasyEncode(unsigned long int frame)
{
    BYTE_BUFFER result = createByteBuffer();
    unsigned int i;
    for(i=0; i<4; i++) {
        pushWord(&result, encodeByte((frame >> ((3 - i) * 8)) & 0xff));
    }
    return result;
}

/**
 * Decode bits with HomeEasy encoding (1 => 10, 0 => 01)
 *
 * @param buffer the buffuer to decode
 *
 * @return new frame
 * */
unsigned long int homeEasyDecode(BYTE_BUFFER *buffer)
{
    unsigned long int result = 0;
    unsigned short int word;
    unsigned char *byte = (unsigned char*)&word;
    unsigned int i;
    for(i=0; i<(buffer->size+1)/2; i++) {
        byte[1] = buffer->data[2*i];
        byte[0] = (2*i+1 < buffer->size ? buffer->data[2*i+1]:0);
        result += (decodeByte(word)) << ((3 - i)* 8);
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
unsigned char decodeByte(unsigned short int word)
{
    unsigned char decodedChar=0;
    int j;
    int shift = 7;
    for(j=0x8000;j>0; j>>=2) {
	    decodedChar |= (((word & j) == j) ? 0x01 : 0x00) << shift;
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
    int shift = 14;
    for(j=0x80;j>0; j>>=1) {
	      encodedChar |= (((byte & j) == j) ? 0x02 : 0x01) << shift;
	      shift -=2;
    }
    return encodedChar;
}

/**
 * Create a complete command according to Chacon protocole
 *
 * @param id command id (refer to your remote)
 * @param section button section ('A' | 'B'  | 'C'  | 'D'  | 'G')
 * @param nb button number(1, 2, 3, 4)
 * @param on boolean for on/off
 * 
 * @return HomeEasy frame
 */
unsigned long int createHomeEasyCommand(unsigned long int id, char section, unsigned char nb, unsigned char on)
{
    unsigned long int command = id << 6;
    char formatedSection = (section<='Z' ? section : section + 'A' - 'a');
    // adding global
    command |= (formatedSection == 'G' ? 0x20 : 0);
    // adding on/off
    command |= (on ? 0x10 : 0);
    // adding section
    command |= ((formatedSection == 'G' ? 0 : formatedSection - 'A') << 2) & 0x0c;
    // adding num
    command |= (formatedSection == 'G' ? 0 : nb-1) & 0x03;;
    return command;
}

/**
 * Send n times a data frame
 *
 * @param frame the data to send
 * @param repeat number of repeatition
 */
void sendFrame(BYTE_BUFFER frame, unsigned int repeat)
{
    unsigned int i;
    // switch to real time
    cpuMode(REAL_TIME);
    // send header
    sendHomeEasyBit(START_OF_DATA);
    sendHomeEasyBit(START_OF_FRAME);
    // repeat the command
    for(i=0; i<repeat; i++) {
        sendHomeEasyBytes(frame);
        sendHomeEasyBit(END_OF_FRAME);
    }
    digitalWrite(homeEasyPinOut, LOW);
    // Exit real time mode
    cpuMode(STANDARD);
}

/**
 * Send a complete command according to Chacon protocole
 *
 * @param id command id (refer to your remote)
 * @param section button section ('A' | 'B'  | 'C'  | 'D'  | 'G')
 * @param nb button number(1, 2, 3, 4)
 * @param on boolean for on/off
 * @param repeat number of repeatition
 */
void sendHomeEasyCommand(unsigned long int id, char section, unsigned char nb, unsigned char on, unsigned char repeat)
{
    unsigned long int command;
    BYTE_BUFFER encoded;
    // build the command
    command = createHomeEasyCommand(id, section, nb, on);
    // encode the command
    encoded = homeEasyEncode(command);
    // send data
    sendFrame(encoded, repeat);
    // release the memory
    destroyByteBuffer(encoded);
}

/**
 * retrieve the HomeEasy device ID from a frame
 *
 * @param buffer the buffer that hold the frame
 *
 * @return the device id
 */
unsigned long int getHomeEasyId(unsigned long int frame)
{
    return frame >> 6;
}

/**
 * Configure the GPIO output pin
 *
 * @param pinNumber wiringPi pin number
 */
void setHomeEasyTransmittorPin(unsigned char pinNumber)
{
    homeEasyPinOut = pinNumber;
}

/**
 * read the GPIO output pin
 *
 * @return wiringPi pin number
 */
unsigned char getHomeEasyTransmittorPin()
{
    return homeEasyPinOut;
}

/**
 * Configure the GPIO input pin
 *
 * @param pinNumber wiringPi pin number
 */
void setHomeEasyReceptorPin(unsigned char pinNumber)
{
    homeEasyPinIn = pinNumber;
}

/**
 * read the GPIO input pin
 *
 * @return wiringPi pin number
 */
unsigned char getHomeEasyReceptorPin()
{
    return homeEasyPinIn;
}

/**
 * Init input/output
 *
 * @return status
 */
int initIO()
{
    int status;
    status = wiringPiSetup();
    if (status != -1) {
        pinMode(homeEasyPinIn, INPUT);
        pinMode(homeEasyPinOut, OUTPUT);
    } else {
        printf("GPIO setup failed %d\n", status);
    }
    return status;
}

/**
 * Send a bit to the RF transmitter
 *
 * @param bit the bit to transmit (0 | 1 | TRIGGER0 | TRIGGER1 | END_OF_FRAME)
 */
void sendHomeEasyBit(unsigned char bit)
{
    digitalWrite(homeEasyPinOut, HIGH);
    delayMicroseconds(timings[bit][0]);
    digitalWrite(homeEasyPinOut, LOW);
    delayMicroseconds(timings[bit][1]);
    digitalWrite(homeEasyPinOut, HIGH);
}

/**
 * Send a byte to the RF transmitter
 *
 * @param byte the byte to transmit
 */
void sendHomeEasyByte(unsigned char byte)
{
    int i;
    for(i=0x80;i>0; i>>=1) {
    	sendHomeEasyBit((byte & i) == i);
    }
}

/**
 * Send the content of a buffer to the RF transmitter
 *
 * @param buffer the buffer to transmit
 */
void sendHomeEasyBytes(BYTE_BUFFER buffer)
{
    unsigned int i;
    for(i=0; i<buffer.size; i++) {
        sendHomeEasyByte(buffer.data[i]);
    }
}

/**
 * Calculate the length of the frame
 *
 * @param data data to scan (each byte represent a bit)
 * @param high length of high level
 * @param low length of low level
 *
 * @return total length of the frame
 */
unsigned int frameSize(unsigned char* data, unsigned int* high, unsigned int* low)
{
    unsigned int i=0;
    if (high) *high = 0;
    if (data[i] == 1) {
        for(i=0; data[i]; i++) ;
        if (high) *high = i;
    }
    for(;  !data[i]; i++) ;
    if (low) *low = i - *high;
    return i;
}

/**
 * Reading data from GPIO
 *
 * @param samples number of samples to read
 * @param duration waiting time between samples
 *
 * @return buffer
 */
BYTE_BUFFER readData(unsigned long int samples, unsigned int duration)
{
    struct timeval* start;
    BYTE_BUFFER result;
    unsigned long int i;

    result = createByteBuffer();
    result.size = samples;
    result.data = (char*) realloc(result.data, samples);

    cpuMode(REAL_TIME);
    start = showTime(0);
    for(i=0; i<samples; i++) {
        result.data[i] = digitalRead(homeEasyPinIn);
        delayMicroseconds(duration);
    }
    showTime(start);
    cpuMode(STANDARD);

    return result;
}
