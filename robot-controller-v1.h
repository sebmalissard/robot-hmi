#ifndef _ROBOT_CONTROLLER_V1_H
#define _ROBOT_CONTROLLER_V1_H

#include <Wire.h>
#include "AX12A.h"

/* General setting */
#define I2C_ADDR                    0x62
#define SERIAL_DEBUG                Serial
#define LOOP_PERIOD                 1   /* in ms */
#define LOOP_POLLING_BUTTON         10  /* in ms; must be greater than LOOP_PERIOD */

/* Device number */
#define LED_NUMBER                  0
#define LED_RGB_NUMBER              0
#define BUTTON_SWITCH_NUMBER        0
#define BUTTON_ROCKER_NUMBER        0
#define DISPLAY_7SEG_4DIG_NUMBER    0
#define AX12_NUMBER                 1
#define ANALOG_NUMBER               2

#define DirectionPin    (7)
#define BaudRate        (1000000ul)

/* Setup pinmux */
#define SETUP_PINMUX()                                  \
{                                                       \
    ax12a.begin(BaudRate, DirectionPin, &Serial1);      \
                                                        \
    ax12[0] = new Ax12(15);                             \
    /* MAX AX12_NUMBER */                               \
                                                        \
    analog[0] = new Analog(A0);                         \
    analog[1] = new Analog(A1);                         \
    /* MAX ANALOG_NUMBER */                             \
}

#endif /* _ROBOT_CONTROLLER_V1_H */
