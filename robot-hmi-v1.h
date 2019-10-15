#ifndef _ROBOT_HMI_V1_H
#define _ROBOT_HMI_V1_H

/* General setting */
#define I2C_ADDR                    0x61
#define SERIAL_DEBUG                Serial
#define LOOP_PERIOD                 1   /* in ms */
#define LOOP_POLLING_BUTTON         10  /* in ms; must be greater than LOOP_PERIOD */

/* Device number */
#define LED_NUMBER                  3
#define LED_RGB_NUMBER              1
#define BUTTON_NUMBER               6
#define DISPLAY_7SEG_4DIG_NUMBER    1
#define IRQ_NUMBER                  1

/* Setup pinmux */
#define SETUP_PINMUX()                                  \
{                                                       \
    led[0] = new Led(A0, false);                        \
    led[1] = new Led(A1, false);                        \
    led[2] = new Led(A2, false);                        \
    /* MAX LED_NUMBER */                                \
                                                        \
    led_rgb[0] = new LedRgb(5, 6, 9, false);            \
    /* MAX LED_RGB_NUMBER */                            \
                                                        \
    button[0] = new Button(7);                          \
    button[1] = new Button(0);                          \
    button[2] = new Button(1);                          \
    button[3] = new Button(4);                          \
    button[4] = new Button(A3);                         \
    button[5] = new Button(14);                         \
    /* MAX BUTTON_NUMBER */                             \
                                                        \
    display_7seg_4dig[0] = new Display7seg4dig();       \
    /* MAX DISPLAY_7SEG_4DIG_NUMBER */                  \
                                                        \
    irq[0] = new  Irq(8);                               \
    /* MAX IRQ_NUMBER */                                \
                                                        \
    display_7seg_4dig[0]->setPower(1);                  \
    display_7seg_4dig[0]->setChar('-', 'H', 'I', '-');  \
}

#endif /* _ROBOT_HMI_V1_H */
