#ifndef _COMMON_H
#define _COMMON_H

#define SERIAL_DEBUG    Serial
#define LOOP_PERIOD     1   /* in ms */
#define LOOP_POLLING_BUTTON 10  /* in ms; must be greater than LOOP_PERIOD */

typedef enum status_t {
    STATUS_OK                 = 0x00,
    STATUS_ERROR              = 0x01,
    STATUS_INVALID_COMMAND    = 0x02,
    STATUS_INVALID_ID         = 0x03,
    STATUS_INVALID_DEVICE     = 0x04,
} status_t;

/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(n, d) ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))

#endif /* _COMMON_H */
