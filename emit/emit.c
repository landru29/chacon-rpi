#include <stdio.h>
#ifdef __arm__
    #include <wiringPiSPI.h>
#endif
#include "emitlib.h"
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define DEFAULT_FREQ 20000



/**
 * Usage of this program
 */
void usage(char** argv)
{
    fprintf(stderr, "Chacon command V1.0\nC. Meichel\n2013, October\n");
    fprintf(stderr, "Syntaxe : %s [options]\n", argv[0]);
    fprintf(stderr, "\t-b button\n\t\tbutton (A-D1-4|G). For instance A1, G, D3\n");
    fprintf(stderr, "\t-x:\n\t\tOff (default On)\n");
    fprintf(stderr, "\t-r number:\n\t\tnumber of repeatitions\n");
    fprintf(stderr, "\t-d id:\n\t\thexadecimal id from your command (13 letters <=> 52 bits)\n");
    fprintf(stderr, "\t-f frequency: configuration frequency for the SPI (default %dHz)\n", DEFAULT_FREQ);
    fprintf(stderr, "\t-v: verbose bits\n");
}



/**
 * Main program
 *
 * @param argc number of arguments passed to the program
 * @param argv array of arguments passed to the program
 *
 * @return status
 */
int main(int argc, char** argv)
{
    BIT_BUFFER buffer;
    int i;
    char optstring[] = "xvb:d:f:r:";
    int option;
    unsigned char global=NO_GLOBAL;
    unsigned char number = NUMBER1;
    unsigned char section = SECTION_A;
    char *idString=0;
    unsigned long int frequency = DEFAULT_FREQ;
    unsigned char onOff = ON;
    unsigned int verbose = 0;
    unsigned repeat = 1;


    /* reading options */
    //opterr=0; /* Pas de message d'erreur automatique */
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'b':
                // Decode the button ie : A3
                section = optarg[0] - (optarg[0]<'Z' ? 'A' : 'a');
                if (strlen(optarg)>1) {
                    number =  optarg[1] - '0';
                }
                if (section== 6) {
                    // G was requested
                    global = GLOBAL;
                }
                break;
            case 'r':
                // repeatition code
                sscanf(optarg, "%d", &repeat);
                break;
            case 'd':
                // command id
                idString = (char*)malloc(1+strlen(optarg)/2);
                for(i=0; i<strlen(optarg)/2 + strlen(optarg) % 2; i++) {
                    idString[i] = (optarg[i*2] - (optarg[i*2]<='9' ? '0' : (optarg[i*2]<='Z' ? 'A' : 'a')-10)) << 4;
                    if (optarg[1+i*2]) {
                        idString[i] += optarg[1+i*2] - (optarg[1+i*2]<='9' ? '0' : (optarg[1+i*2]<='Z' ? 'A' : 'a')-10);
                    }
                }
                break;
            case 'f':
                // SPI frequency
                sscanf(optarg, "%lu", &frequency);
                break;
            case 'x':
                // OFF ?
                onOff = OFF;
                break;
            case 'v':
                // Show the bits to send ?
                verbose = 1;
                break;
            default:
                usage(argv);
                return 0;
                break;
        }
    }

    // No ID specified, it will be "00 00 00 00 00 00 0"
    if (!idString) {
        idString = (char*)malloc(7);
        memset(idString, 0, 7);
    }

    // Show informations
    printf("Frequency config on SPI: %dHz\n", frequency);

    if (global) {
        printf("Sending command G: %s\n", (onOff == OFF) ? "OFF" : "ON");
    } else {
        printf("Sending command %c%d: %s\n", section+'A', number, (onOff == OFF) ? "OFF" : "ON");
    }

    // Preparing the buffer
    buffer = createBitBuffer();

    // Building the data
    for (i=0; i<repeat; i++) {
        pushCode(&buffer, idString, section, number, onOff, global, frequency);
    }
    printf("\n");

    // Want to see the bits ?
    if (verbose) {
        printfBitBuffer(buffer);
    }

#ifdef __arm__
    // preparing the output
    if (wiringPiSPISetup(0, frequency) < 0) {
        printf("SPI Setup Failed: %s\n", strerror(errno));
        return -1;
    }
    // Send data
    wiringPiSPIDataRW(0, (unsigned char*)buffer.data, buffer.byteSize);
#endif

    // release memory
    destroyBitBuffer(buffer);
    return 0;
}
