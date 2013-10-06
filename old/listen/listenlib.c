#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include "listenlib.h"


/**
 * Calculate a duration
 *
 * @param start start of the computing or null if
 * this is the first time this function is inoked
 *
 * @return current time
 */
struct timeval* showTime(struct timeval* start)
{
    struct timeval* stop;
    long tv_sec;
    long tv_usec;
    stop = (struct timeval*)malloc(sizeof(struct timeval));
    gettimeofday(stop, 0);
    if (!start) return stop;
    tv_sec = (start->tv_usec > stop->tv_usec) ? stop->tv_sec - start->tv_sec - 1 : stop->tv_sec - start->tv_sec;
    tv_usec = (start->tv_usec > stop->tv_usec) ? 1000000 + stop->tv_usec - start->tv_usec : stop->tv_usec - start->tv_usec;
    fprintf(stderr, "%lus %lums %luµs\n", tv_sec, tv_usec/1000, tv_sec % 1000);
    return stop;
}

/**
 * Reading data from GPIO
 *
 * @param data buffer data
 * @param samples number of samples to read
 * @param duration waiting time between samples
 */
void readData(unsigned char* data, unsigned long int samples, unsigned int duration)
{
    struct timeval* start;
    unsigned long int i;
    start = showTime(0);
    for(i=0; i<samples; i++) {
        data[i] = digitalRead(LED);
        usleep(duration);
    }
    data[samples] = 123;
    showTime(start);
}

/**
 * Calculate the length of the frame
 *
 * @param data data to scan
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
 * Analyse the data
 *
 * @param data buffer data
 * @param samples number of samples to read
 * @param output output to render the result
 * @param all if false the analyze stops at the first error encoutered
 */
void analyse(unsigned char* data, unsigned long int samples, FILE* output, unsigned char all)
{
    unsigned long int cursor=0;
    int detectMarker = 1;
    int currentMarker = 0;
    unsigned int lowLengthFrame;
    unsigned int highLengthFrame;
    unsigned int totalLengthFrame;
    unsigned char currentByte = 0;
    unsigned char bitNumber = 0;
    unsigned char startData = 0;
    while (cursor<samples) {
        totalLengthFrame = frameSize(&data[cursor], &highLengthFrame, &lowLengthFrame);
        if (currentMarker>0) {
            /* Try to detect a start frame marker */
            if ((lowLengthFrame > 20) && (lowLengthFrame<50)) {
                detectMarker++;
                bitNumber = 0;
                currentByte = 0;
                fprintf(output, "\n%02d: ", detectMarker);
            } else {
                currentMarker = detectMarker;
            }
            /* we are in the data range */
            if (currentMarker == detectMarker) {
                /* Push bit */
                currentByte = currentByte << 1;
                if ((lowLengthFrame<2) || (highLengthFrame>5)) {
                    fprintf(output, "There might be an error from here\n");
                    if (!all) return;
		}
                currentByte += (lowLengthFrame < 5 ? 0 : 1);
                /* check if the byte is completed */
                if (bitNumber == 7) {
                    bitNumber = 0;
                    fprintf(output, "%02X ", currentByte);
                    currentByte = 0;
                } else {
                   bitNumber++;
                }
            }
            /* We are in the data range */
        } else {
            if ((lowLengthFrame > 20) && (lowLengthFrame<50) && (startData)) {
                currentMarker = 1;
                currentByte = 0;
                bitNumber = 0;
                fprintf(output, "DataStart at %lu\n", cursor);
                fprintf(output, "\n%02d: ", detectMarker);
            }
            if (lowLengthFrame > 100) {
                startData = 1;
            }
        }
        cursor += totalLengthFrame;
    }
}

