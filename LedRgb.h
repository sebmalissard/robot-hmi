#ifndef _LED_RGB_H
#define _LED_RGB_H

#include "common.h"

class LedRgb
{
    public:
        LedRgb(uint8_t pin_red, uint8_t pin_green, uint8_t pin_blue, bool is_common_cathode);
        
        void loop();
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getPower(uint8_t *power);
        status_t setPower(uint8_t power);
        
        status_t getColor(uint8_t *color);
        status_t setColor(uint8_t color);
        
        status_t getPwm(uint8_t *pwm_red, uint8_t *pwm_green, uint8_t *pwm_blue);
        status_t setPwm(uint8_t pwm_red, uint8_t pwm_green, uint8_t pwm_blue);
        
        status_t getBlink(uint16_t *period, uint8_t *duty_cycle);
        status_t setBlink(uint16_t period, uint8_t duty_cycle);
    
    private:
        void ledSetValue(bool value);
        
        bool        _force_update;
        uint8_t     _pin_red;
        uint8_t     _pin_green;
        uint8_t     _pin_blue;
        bool        _is_common_cathode;
        bool        _power;
        uint8_t     _pwm_red;
        uint8_t     _pwm_green;
        uint8_t     _pwm_blue;
        uint16_t    _blink_period;
        uint8_t     _blink_duty_cycle;
};

#endif /* _LED_RGB_H */
