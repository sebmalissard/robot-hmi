#ifndef _COMMON_H
#define _COMMON_H

#define SERIAL_DEBUG    Serial
#define LOOP_PERIOD     1 /* in ms */

typedef enum status_t {
    OK,
    ERROR,
    STATUS_OK                  = 0x00,
    STATUS_ERROR               = 0x01,
    INVALID_COMMAND     = 0x02,
    UNSUPPORTED_ID      = 0x03,
    UNSUPPORTED_COMMAND = 0x04,
} status_t;

/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(n, d) ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))

#endif /* _COMMON_H */
