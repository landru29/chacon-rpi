#include <stdio.h>
#include "../home_easy.h"
#include "../buffer.h"

#define TEST_START printf("\n=== %s ===\n", __FUNCTION__)


void testByteBuffer()
{
    BYTE_BUFFER buffer;
    char bytes[] = {0xcd, 0xef, 0x12};
    TEST_START;

    buffer = createByteBuffer();

    pushByte(&buffer, 0xab);
    pushBytes(&buffer, bytes, 3);
    pushWord(&buffer, 0x3456);

    printf("Should: AB CD EF 12 34 56\nGet: ");
    printfByteBuffer(buffer);

    destroyByteBuffer(buffer);
}

void testPrintBits()
{
    BYTE_BUFFER buffer;
    char bytes[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    TEST_START;

    buffer = createByteBuffer();
    pushBytes(&buffer, bytes, 8);

    printfBitBuffer(buffer);

    destroyByteBuffer(buffer);
}

void testEncode()
{
    unsigned char srcByte = 0x28;
    unsigned short int encWord;
    TEST_START;

    encWord = encodeByte(srcByte);
    printf("Input: %02X\nShould: 0x5995\nGet: %04X\n", srcByte, encWord);
}


void testDecode()
{
    unsigned short int srcWord = 0x5995;
    unsigned char decByte;
    TEST_START;

    decByte = decodeByte(srcWord);
    printf("Input: %04X\nShould: 0x28\nGet: %02X\n", srcWord, decByte);
}

void testHomeEasyEncode()
{
    BYTE_BUFFER source;
    BYTE_BUFFER encoded;
    unsigned char bytes[] = {0x28, 0x28, 0x01};
    TEST_START;

    source = createByteBuffer();
    pushBytes(&source, bytes, 3);
    printf("Input: ");
    printfByteBuffer(source);
    printf("Should: 59 95 59 95 55 56\nGet:    ");

    encoded = homeEasyEncode(&source);
    printfByteBuffer(encoded);

    destroyByteBuffer(source);
    destroyByteBuffer(encoded);
}

void testHomeEasyDecode()
{
    BYTE_BUFFER source;
    BYTE_BUFFER decoded;
    unsigned char bytes[] = {0x59, 0x95, 0x59, 0x95, 0x55, 0x56};
    TEST_START;

    source = createByteBuffer();
    pushBytes(&source, bytes, 6);
    printf("Input: ");
    printfByteBuffer(source);
    printf("Should: 28 28 01\nGet:    ");

    decoded = homeEasyDecode(&source);
    printfByteBuffer(decoded);

    destroyByteBuffer(source);
    destroyByteBuffer(decoded);
}


void testHomeEasyCommand(char section, unsigned char num, unsigned char onOff)
{
    BYTE_BUFFER command;
    BYTE_BUFFER encoded;
    unsigned char id[] = {0x28, 0x28, 0x01, 0x80};
    TEST_START;

    command = createHomeEasyCommand(id, section, num, onOff);
    printfByteBuffer(command);

    encoded = homeEasyEncode(&command);
    printfByteBuffer(encoded);

    destroyByteBuffer(command);
    destroyByteBuffer(encoded);
}

int main()
{
    printf("Test\n");

    testByteBuffer();

    testPrintBits();

    testEncode();

    testDecode();

    testHomeEasyEncode();

    testHomeEasyDecode();

    testHomeEasyCommand('D', 4, OFF);

    return 0;
}
