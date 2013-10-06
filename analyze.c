#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include "home_easy.h"
#include "analyze.h"


/**
 * Analyse the data
 *
 * @param buffer data to analyze
 * @param output output to render the result
 * @param all if false the analyze stops at the first error encoutered
 */
void analyse(BYTE_BUFFER buffer, FILE* output, unsigned char all)
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
    unsigned char* data = buffer.data;
    unsigned char extractedSection;
    unsigned char extractedNumber;
    unsigned char extractedOnOff;
    BYTE_BUFFER readFrame;
    unsigned long int readDWord;
    readFrame = createByteBuffer();
    while (cursor<buffer.size) {
        totalLengthFrame = frameSize(&data[cursor], &highLengthFrame, &lowLengthFrame);
        if (currentMarker>0) {
            /* Try to detect a start frame marker */
            if ((lowLengthFrame > 20) && (lowLengthFrame<50)) {
                detectMarker++;
                bitNumber = 0;
                currentByte = 0;
                if (readFrame.size == 8) {
                    readDWord = homeEasyDecode(&readFrame);
                    getHomeEasyInfo(readDWord, 0, &extractedOnOff, &extractedSection, &extractedNumber);
                    fprintf(output, "\nDecoded command: %08X\nID found: %08X\n", readDWord, getHomeEasyId(readDWord));
                    fprintf(output, "Button %c%d - %s pressed", extractedSection, extractedNumber, (extractedOnOff ? "OFF" : "ON"));
                } else {
                    fprintf(output, "Nothing to decode with %d byte(s)\n", readFrame.size);
                }
                readFrame.size = 0;
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
                    pushByte(&readFrame, currentByte);
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
    destroyByteBuffer(readFrame);
}

