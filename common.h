#ifndef _COMMON_H
#define _COMMON_H

#define SERIAL_DEBUG Serial

typedef enum status_t {
    OK,
    ERROR,
    STATUS_OK                  = 0x00,
    STATUS_ERROR               = 0x01,
    INVALID_COMMAND     = 0x02,
    UNSUPPORTED_ID      = 0x03,
    UNSUPPORTED_COMMAND = 0x04,
} status_t;

#endif /* _COMMON_H */
