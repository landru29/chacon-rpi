#include <stdio.h>
#include "buffer.h"

/**
 * Analyse the data
 *
 * @param buffer data to analyze
 * @param output output to render the result
 * @param all if false the analyze stops at the first error encoutered
 */
void analyse(BYTE_BUFFER buffer, FILE* output, unsigned char all);
