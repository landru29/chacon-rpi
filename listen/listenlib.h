#include <stdio.h>
#include <sys/time.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
#define	LED	0


/**
 * Calculate a duration
 *
 * @param start start of the computing or null if
 * this is the first time this function is inoked
 *
 * @return current time
 */
struct timeval* showTime(struct timeval* start);

/**
 * Reading data from GPIO
 *
 * @param data buffer data
 * @param samples number of samples to read
 * @param duration waiting time between samples
 */
void readData(unsigned char* data, unsigned long int samples, unsigned int duration);

/**
 * Calculate the length of the frame
 *
 * @param data data to scan
 * @param high length of high level
 * @param low length of low level
 *
 * @return total length of the frame
 */
unsigned int frameSize(unsigned char* data, unsigned int* high, unsigned int* low);

/**
 * Analyse the data
 *
 * @param data buffer data
 * @param samples number of samples to read
 * @param output output to render the result
 * @param all if false the analyze stops at the first error encoutered
 */
void analyse(unsigned char* data, unsigned long int samples, FILE* output, unsigned char all);
