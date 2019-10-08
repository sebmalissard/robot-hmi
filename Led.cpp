#include "Led.h"

typedef enum led_cmd_t
{
    POWER       = 0x01,
    INTENSITY   = 0x02,
    BLINK       = 0x03,
} led_cmd_t;


Led::Led(uint8_t pin, bool is_pwm): _pin(pin), _is_pwm(_is_pwm)
{
}

status_t Led::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((led_cmd_t) cmd)
    {
        case POWER:
            *size = 1;
            return getPower(&arg[0]);
            
        case INTENSITY:
            *size = 1;
            return getIntensity(&arg[0]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Led get command");
    }

    return INVALID_COMMAND;
}

status_t Led::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    switch ((led_cmd_t) cmd)
    {
        case POWER:
             if (size == 1)
                return setPower(arg[0]);
             break;
        
        case INTENSITY:
            if (size == 1)
                return setIntensity(arg[0]);
            break;
        
        default:
            SERIAL_DEBUG.println("Unsupported Led set command");
    }
    
    return INVALID_COMMAND;
}



status_t Led::getPower(uint8_t *power)
{
    if (!power)
    {
        return STATUS_ERROR;
    }
    
    *power = _power;
    
    return STATUS_OK;
}

status_t Led::setPower(uint8_t power)
{
    _power = power ? true : false;
    
    if (_is_pwm)
    {
        analogWrite(_pin, _power ? _intensity : 0);
    }
    else
    {
        digitalWrite(_pin, _power);
    }
    
    return STATUS_OK;
}
