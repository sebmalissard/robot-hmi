#ifndef _BUTTON_ROCKER_H
#define _BUTTON_ROCKER_H

#include "common.h"

class ButtonRocker
{
    public:
        ButtonRocker(uint8_t pin);
        
        bool irq();
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getState(uint8_t *state);
    
    private:
        uint8_t     _pin;
        bool        _state;
};

#endif /* _BUTTON_ROCKER_H */
