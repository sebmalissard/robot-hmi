#ifndef _ANALOG_H
#define _ANALOG_H

#include "Device.h"

class Analog : public Device
{
    public:
        Analog(uint8_t pin);
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getVoltage(uint8_t *voltage_msb, uint8_t *voltage_lsb);
    
    private:
        void ledSetValue(bool value);
        
        uint8_t     _pin;
};

#endif /* _ANALOG_H */
