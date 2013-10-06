#ifndef __HOME_EASY_H__
#define __HOME_EASY_H__

#include <stdio.h>
#include "buffer.h"

#define BIT0 0
#define BIT1 1
#define START_OF_DATA 2
#define START_OF_FRAME 3
#define END_OF_FRAME 4

#define ON 1
#define OFF 0

/**
 * Encode bits with HomeEasy encoding (1 => 10, 0 => 01)
 *
 * @param frame 32-bit frame to encode
 *
 * @return new buffer
 * */
BYTE_BUFFER homeEasyEncode(unsigned long int frame);

/**
 * Decode bits with HomeEasy encoding (1 => 10, 0 => 01)
 *
 * @param buffer the buffuer to decode
 *
 * @return new frame
 * */
unsigned long int homeEasyDecode(BYTE_BUFFER *buffer);

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
 * Create a complete command according to Chacon protocole
 *
 * @param id command id (refer to your remote)
 * @param section button section ('A' | 'B'  | 'C'  | 'D'  | 'G')
 * @param nb button number(1, 2, 3, 4)
 * @param on boolean for on/off
 * 
 * @return HomeEasy frame
 */
unsigned long int createHomeEasyCommand(unsigned long int id, char section, unsigned char nb, unsigned char on);

/**
 * Send a complete command according to Chacon protocole
 *
 * @param id command id (refer to your remote)
 * @param section button section ('A' | 'B'  | 'C'  | 'D'  | 'G')
 * @param nb button number(1, 2, 3, 4)
 * @param on boolean for on/off
 * @param repeat number of repeatition
 */
void sendHomeEasyCommand(unsigned long int id, char section, unsigned char nb, unsigned char on, unsigned char repeat);

/**
 * Send n times a data frame
 *
 * @param frame the data to send
 * @param repeat number of repeatition
 */
void sendFrame(BYTE_BUFFER frame, unsigned int repeat);

/**
 * Configure the GPIO output pin
 *
 * @param pinNumber wiringPi pin number
 */
void setHomeEasyTransmittorPin(unsigned char pinNumber);

/**
 * Init input/output
 *
 * @return status
 */
int initIO();

/**
 * retrieve the HomeEasy device ID from a frame
 *
 * @param buffer the buffer that hold the frame
 *
 * @return the device id
 */
unsigned long int getHomeEasyId(unsigned long int frame);

/**
 * Get all information about the homeEasy frame
 *
 * @param frame the frame to read
 * @param id the identifier to extract
 * @param onOff boolean to extract; if true : on
 * @param section letter section to extract
 * @param number number to extract
 */
void getHomeEasyInfo(unsigned long int frame, unsigned long int* id, unsigned char* onOff, unsigned char* section, unsigned char* number);

/**
 * read the GPIO output pin
 *
 * @return wiringPi pin number
 */
unsigned char getHomeEasyTransmittorPin();

/**
 * Configure the GPIO input pin
 *
 * @param pinNumber wiringPi pin number
 */
void setHomeEasyReceptorPin(unsigned char pinNumber);

/**
 * read the GPIO input pin
 *
 * @return wiringPi pin number
 */
unsigned char getHomeEasyReceptorPin();

/**
 * Send a bit to the RF transmitter
 * 
 * @param bit the bit to transmit (0 | 1 | TRIGGER0 | TRIGGER1 | END_OF_FRAME)
 */
void sendHomeEasyBit(unsigned char bit);

/**
 * Send a byte to the RF transmitter
 * 
 * @param byte the byte to transmit
 */
void sendHomeEasyByte(unsigned char byte);

/**
 * Send the content of a buffer to the RF transmitter
 * 
 * @param buffer the buffer to transmit
 */
void sendHomeEasyBytes(BYTE_BUFFER buffer);

/**
 * Calculate the length of the frame
 *
 * @param data data to scan (each byte represent a bit)
 * @param high length of high level
 * @param low length of low level
 *
 * @return total length of the frame
 */
unsigned int frameSize(unsigned char* data, unsigned int* high, unsigned int* low);

/**
 * Reading data from GPIO
 *
 * @param samples number of samples to read
 * @param duration waiting time between samples
 *
 * @return buffer
 */
BYTE_BUFFER readData(unsigned long int samples, unsigned int duration);

 #endif
