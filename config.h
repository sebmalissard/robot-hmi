#ifndef _CONFIG_H
#define _CONFIG_H

/* Select the config to include */
// #define CONFIG_...
#if defined(CONFIG_ROBOT_HMI_V1)
#include "robot-hmi-v1.h"
#elif defined(CONFIG_ROBOT_CONTROLLER_V1)
#include "robot-controller-v1.h"
#else
#error "You need to define the config to use"
#endif

/* Default value if undefine */
#ifndef LED_NUMBER
#define LED_NUMBER 0
#endif

#ifndef LED_RGB_NUMBER
#define LED_RGB_NUMBER 0
#endif

#ifndef BUTTON_SWITCH_NUMBER
#define BUTTON_SWITCH_NUMBER 0
#endif

#ifndef BUTTON_ROCKER_NUMBER
#define BUTTON_ROCKER_NUMBER 0
#endif

#ifndef AX12_NUMBER
#define AX12_NUMBER 0
#endif

#ifndef DISPLAY_7SEG_4DIG_NUMBER
#define DISPLAY_7SEG_4DIG_NUMBER 0
#endif

#endif /* _CONFIG_H */
