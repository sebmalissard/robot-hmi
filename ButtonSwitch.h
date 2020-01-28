#ifndef _BUTTON_SWITCH_H
#define _BUTTON_SWITCH_H

#include "Device.h"

class ButtonSwitch : public Device
{
    public:
        ButtonSwitch(uint8_t pin);
        
        bool irq_polling();
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getState(uint8_t *state);
    
    private:
        uint8_t     _pin;
        bool        _state;
        uint8_t     _irq_polling_last_state;
};

#endif /* _BUTTON_SWITCH_H */
