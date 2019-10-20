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
#define BUTTON_SWITCH_NUMBER        3
#define BUTTON_ROCKER_NUMBER        3
#define DISPLAY_7SEG_4DIG_NUMBER    1

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
    button_switch[0] = new ButtonSwitch(0);             \
    button_switch[1] = new ButtonSwitch(1);             \
    button_switch[2] = new ButtonSwitch(4);             \
    /* MAX BUTTON_SWITCH_NUMBER */                      \
                                                        \
    button_rocker[0] = new ButtonRocker(7);             \
    button_rocker[1] = new ButtonRocker(A3);            \
    button_rocker[2] = new ButtonRocker(14);            \
    /* MAX BUTTON_ROCKER_NUMBER */                      \
                                                        \
    display_7seg_4dig[0] = new Display7seg4dig();       \
    /* MAX DISPLAY_7SEG_4DIG_NUMBER */                  \
                                                        \
    irq[0] = new  Irq(8);                               \
    /* MAX IRQ_NUMBER */                                \
                                                        \
    irq_callback[0] = button_switch[0];                 \
    irq_callback[1] = button_switch[1];                 \
    irq_callback[2] = button_switch[2];                 \
    irq_callback[3] = button_rocker[0];                 \
    irq_callback[4] = button_rocker[1];                 \
    irq_callback[5] = button_rocker[2];                 \
    /* MAX IRQ_REG_SIZE */                              \
                                                        \
    display_7seg_4dig[0]->setPower(1);                  \
    display_7seg_4dig[0]->setChar('-', 'H', 'I', '-');  \
}

#endif /* _ROBOT_HMI_V1_H */
