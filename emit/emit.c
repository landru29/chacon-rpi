#include <stdio.h>
#ifdef __arm__
    #include <wiringPiSPI.h>
#endif
#include "home_easy.h"
#include "buffer.h"
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


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
    //BIT_BUFFER buffer;
    BYTE_BUFFER command;
    BYTE_BUFFER encoded;
    int i;
    char optstring[] = "xvb:d:r:";
    int option;
    unsigned char number = 1;
    unsigned char section = 'A';
    char *idString=0;
    unsigned long int id=0;
    unsigned char onOff = ON;
    unsigned int verbose = 0;
    unsigned repeat = 5;


    /* reading options */
    //opterr=0; /* Pas de message d'erreur automatique */
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'b':
                // Decode the button ie : A3
                section = optarg[0] - (optarg[0]<'Z' ? 0 : 'a' - 'A');
                if (strlen(optarg)>1) {
                    number =  optarg[1] - '0';
                }
                break;
            case 'r':
                // repeatition code
                sscanf(optarg, "%d", &repeat);
                break;
            case 'd':
                // command id
                sscanf(optarg, "%08X", &id);
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

    // Show informations
    printf("device id: %08X\n", id);
    if (section == 'G') {
        printf("Sending command G: %s\n", (onOff == OFF) ? "OFF" : "ON");
    } else {
        printf("Sending command %c%d: %s\n", section, number, (onOff == OFF) ? "OFF" : "ON");
    }

    // Display mor information
    if (verbose) {
        command = createHomeEasyCommand(id, section, number, onOff);
        printf("Frame to send: %08X\n", command);

        printf("Code to emit:\n");
        encoded = homeEasyEncode(&command);
        printfByteBuffer(encoded);
    }

    // Send the data
    initIO();
    sendHomeEasyCommand(id, section, number, onOff, repeat);

    // release the memory
    destroyByteBuffer(command);
    destroyByteBuffer(encoded);

    return 0;
}
