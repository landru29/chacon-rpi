#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

/**
 * Calculate a duration
 *
 * @param start start of the computing or null if
 * this is the first time this function is inoked
 *
 * @return current time
 */
struct timeval* showTime(struct timeval* start);

#endif