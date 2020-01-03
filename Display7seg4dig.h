#ifndef _DISPLAY_7SEG_4DIG_H
#define _DISPLAY_7SEG_4DIG_H

#include "Max7219.h"
#include "Device.h"

class Display7seg4dig : public Device
{
    public:
        Display7seg4dig();
        
        void loop();
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getPower(uint8_t *power);
        status_t setPower(uint8_t power);
        
        status_t getIntensity(uint8_t *intensity);
        status_t setIntensity(uint8_t intensity);
        
        status_t setChar(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3);
        status_t setRaw(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3);
        status_t setInteger(int16_t integer);
        
        status_t getBlink(uint16_t *period, uint8_t *duty_cycle);
        status_t setBlink(uint16_t period, uint8_t duty_cycle);
    
    private:
        void displaySetValue(bool value);
        
        enum printFunc {CHAR, RAW, INTEGER}; 
        
        bool        _force_update;
        Max7219     _display;
        printFunc   _print_func;
        bool        _power;
        uint8_t     _intensity;
        uint8_t     _char[4];
        uint8_t     _raw[4];
        int16_t     _integer;
        uint16_t    _blink_period;
        uint8_t     _blink_duty_cycle;
        uint16_t    _loop_i;
        bool        _loop_last_value;
};

#endif /* _DISPLAY_7SEG_4DIG_H */
