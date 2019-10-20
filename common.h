#ifndef _COMMON_H
#define _COMMON_H

#include <Arduino.h>

#include "config.h"

typedef enum status_t {
    STATUS_OK               = 0x00,
    STATUS_ERROR            = 0x01,
    STATUS_INVALID_PACKET   = 0x02,
    STATUS_INVALID_COMMAND  = 0x03,
    STATUS_INVALID_ID       = 0x04,
    STATUS_INVALID_DEVICE   = 0x05,
    STATUS_INVALID_ARGUMENT = 0x06,
} status_t;

typedef enum device_t {
    SYSTEM                  = 0x00,
    GPIO                    = 0x01,
    LED                     = 0x02,
    LED_RGB                 = 0x03,
    BUTTON_SWITCH           = 0x04,
    BUTTON_ROCKER           = 0x05,
    SERVO                   = 0x06,
    AX12                    = 0x07,
    DISPLAY_7SEG_4DIG       = 0x08,
    MOTOR                   = 0x09,
    IRQ                     = 0xFF
} device_t;

inline const char *deviceToString(device_t device)
{
    switch (device)
    {
        case SYSTEM:            return "SYSTEM";
        case GPIO:              return "GPIO";
        case LED:               return "LED";
        case LED_RGB:           return "LED_RGB";
        case BUTTON_SWITCH:     return "BUTTON_SWITCH";
        case BUTTON_ROCKER:     return "BUTTON_ROCKER";
        case SERVO:             return "SERVO";
        case AX12:              return "AX12";
        case DISPLAY_7SEG_4DIG: return "DISPLAY_7SEG_4DIG";
        case MOTOR:             return "MOTOR";
        case IRQ:               return "IRQ";
    }
    return "UNKNOWN";
}

/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(n, d) ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))

#define INVALID_ARGUMENT(msg)               \
{                                           \
    SERIAL_DEBUG.println(msg);              \
    return STATUS_INVALID_ARGUMENT;         \
}

#define IRQ_REG_SIZE  8
#define IRQ_NUMBER    1

#endif /* _COMMON_H */
