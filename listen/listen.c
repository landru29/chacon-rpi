#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include "listenlib.h"

/**
 * Usage of this program
 */
void usage(char** argv)
{
    fprintf(stderr, "Chacon analyzer V1.0\nC. Meichel\n2013, October\n");
    fprintf(stderr, "Syntaxe : %s [options]\n", argv[0]);
    fprintf(stderr, "\t-o output:\n\t\toutput file\n");
    fprintf(stderr, "\t-n number:\n\t\tnumber of bit to analyze (default 100000)\n");
    fprintf(stderr, "\t-t number:\n\t\ttempo between samples in µs (default 10)\n");
    fprintf(stderr, "\t-v:\n\t\tverbose\n");
    fprintf(stderr, "\t-a:\n\t\tperform a complete analyse even if an error was encountered\n");
}

/**
 * Main program
 *
 * @param argc number of arguments passed to the program
 * @param argv array of arguments passed to the program
 *
 * @return status
 */
int main (int argc, char** argv)
{
    char optstring[] = "n:t:o:va";
    unsigned int i;
    unsigned char previousBit;
    unsigned char* data;
    struct timeval* start;
    int option;
    unsigned int duration = 10;
    unsigned long int samples = 100000;
    unsigned char verbose = 0;
    unsigned char all = 0;
    FILE* output=0;

    /* reading options */
    opterr=0; /* Pas de message d'erreur automatique */
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 't':
                sscanf(optarg, "%d", &duration);
                break;
            case 'n':
                sscanf(optarg, "%lu", &samples);
                break;
            case 'a':
                all = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'o':
                output = fopen(optarg, "w");
                if (output==0) {
                    fprintf(stderr, "Could not open file %s\n", optarg);
                }
                break;
            default:
                usage(argv);
                return 0;
                break;
        }
    }

    /* Configure the GPIO */
    wiringPiSetup () ;
    pinMode (LED, INPUT);

    /* Read the data */
    fprintf(stderr, "Reading data\n");
    data = (unsigned char*)malloc(samples);
    readData(data, samples, duration);

    /* Analyzing the data */
    fprintf(stderr, "Analyzing\n");
    analyse(data, samples, output ? output : stdout, all);

    if (verbose) {
        fprintf(stdout, "\n\nRawData\n");
        previousBit=0;
        for(i=0; i<samples; i++) {
           if ((previousBit == 0) && (data[i] == 1))
               fprintf(stdout, "\n");
           fprintf(stdout, "%c", data[i]+'0');
           previousBit = data[i];
       }
    }

    free(data);
    return 0 ;
}
