#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdarg.h>
#include <stdint.h>

#include <Arduino.h>

#include "common.h"

class Button
{
    public:
        Button(uint8_t pin);
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getState(uint8_t *state);
    
    private:
        uint8_t     _pin;
        bool        _state;
};

#endif /* _BUTTON_H */
