#ifndef _LED_H
#define _LED_H

#include <stdarg.h>
#include <stdint.h>

#include <Arduino.h>

#include "common.h"

class Led
{
    public:
        Led(uint8_t pin, bool is_pwm);
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getPower(uint8_t *power);
        status_t setPower(uint8_t power);
        
        status_t getIntensity(uint8_t *intensity);
        status_t setIntensity(uint8_t intensity);
    
    private:
        uint8_t     _pin;
        bool        _is_pwm;
        bool        _power;
        uint8_t     _intensity;
        uint16_t    _blink_period;
        uint8_t     _blink_duty_cycle;
};

#endif /* _LED_H */