#ifndef _AX12_H
#define _AX12_H

#include "Device.h"

class Ax12 : public Device
{
    public:
        Ax12(uint8_t id);
        
        
        void loop();
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getPowerLed(uint8_t *power_led);
        status_t setPowerLed(uint8_t power_led);
        
        status_t getPosition(uint16_t *position);
        status_t setPosition(uint16_t position);
    
    private:
        void ax12SetValue();
        
        bool        _force_update;
        uint8_t     _id;
        bool        _power_led;
        uint16_t    _position;
};

#endif /* _AX12_H */
