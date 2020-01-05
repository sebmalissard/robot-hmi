#include "Display7seg4dig.h"

typedef enum display_7seg_4dig_cmd_t
{
    POWER       = 0x01,
    INTENSITY   = 0x02,
    CHAR        = 0x03,
    RAW         = 0x04,
    INTEGER     = 0x05,
    BLINK       = 0x06,
} display_7seg_4dig_cmd_t;

Display7seg4dig::Display7seg4dig(): _force_update(true), _display(4), _power(LOW), _print_func(RAW), _intensity(128),
    _char({0, 0, 0, 0}), _raw({0, 0, 0, 0}), _integer(0), _blink_period(0), _blink_duty_cycle(0),
    _loop_i(0), _loop_last_value(LOW)
{
    displaySetValue(_power);
}


void Display7seg4dig::loop()
{   
    if (_force_update)
    {
        displaySetValue(_power);
        _force_update = false;
    }
    
    if (_power)
    {
        if (_loop_i < DIV_ROUND_CLOSEST((uint32_t)_blink_period * _blink_duty_cycle / 255, LOOP_PERIOD))
        {
            if (_loop_last_value != HIGH)
            {
                displaySetValue(HIGH);
                _loop_last_value = HIGH;
            }
        }
        else
        {
            if (_loop_last_value != LOW)
            {
                displaySetValue(LOW);
                _loop_last_value = LOW;
            }
        }
        
        _loop_i++;
        
        if (_loop_i >= _blink_period / LOOP_PERIOD)
        {
            _loop_i = 0;
        }
    }
}

status_t Display7seg4dig::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((display_7seg_4dig_cmd_t) cmd)
    {
        case display_7seg_4dig_cmd_t::POWER:
            *size = 1;
            return getPower(&arg[0]);
            
        case display_7seg_4dig_cmd_t::INTENSITY:
            *size = 1;
            return getIntensity(&arg[0]);
            
        case display_7seg_4dig_cmd_t::BLINK:
            *size = 3;
            return getBlink((uint16_t*) &arg[0], &arg[2]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Display 7seg 4dig get command.");
    }

    return STATUS_INVALID_COMMAND;
}

status_t Display7seg4dig::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    switch ((display_7seg_4dig_cmd_t) cmd)
    {
        case display_7seg_4dig_cmd_t::POWER:
             if (size == 1)
                return setPower(arg[0]);
            else
                INVALID_ARGUMENT("Display 7seg 4dig Power invalid number of argument.");
            
        case display_7seg_4dig_cmd_t::INTENSITY:
            if (size == 1)
                return setIntensity(arg[0]);
            else
                INVALID_ARGUMENT("Display 7seg 4dig Intensity invalid number of argument.");
            
        case display_7seg_4dig_cmd_t::CHAR:
            if (size == 4)
                return setChar(arg[0], arg[1], arg[2], arg[3]);
            else
                INVALID_ARGUMENT("Display 7seg 4dig Cahr invalid number of argument.");
            
        case display_7seg_4dig_cmd_t::RAW:
            if (size == 4)
                return setRaw(arg[0], arg[1], arg[2], arg[3]);
            else
                INVALID_ARGUMENT("Display 7seg 4dig Raw invalid number of argument.");

        case display_7seg_4dig_cmd_t::INTEGER:
            if (size == 2)
                return setInteger((arg[0] << 8) + arg[1]);
            else
                INVALID_ARGUMENT("Display 7seg 4dig Integer invalid number of argument.");
            
        case display_7seg_4dig_cmd_t::BLINK:
            if (size == 3)
                return setBlink((arg[0] << 8) + arg[1], arg[2]);
            else
                INVALID_ARGUMENT("Display 7seg 4dig Blink invalid number of argument.");
            
        default:
            SERIAL_DEBUG.println("Unsupported Display 7seg 4dig set command.");
    }
    
    return STATUS_INVALID_COMMAND;
}

status_t Display7seg4dig::getPower(uint8_t *power)
{
    if (!power)
    {
        return STATUS_ERROR;
    }
    
    *power = _power;
    
    return STATUS_OK;
}

status_t Display7seg4dig::setPower(uint8_t power)
{
    _power = power ? HIGH : LOW;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Display7seg4dig::getIntensity(uint8_t *intensity)
{
    if (!intensity)
    {
        return STATUS_ERROR;
    }
    
    *intensity = _intensity;
    
    return STATUS_OK;
}

status_t Display7seg4dig::setIntensity(uint8_t intensity)
{
    _intensity = intensity;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Display7seg4dig::setChar(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3)
{
    _print_func = CHAR;
    
    _char[0] = dig0;
    _char[1] = dig1;
    _char[2] = dig2;
    _char[3] = dig3;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Display7seg4dig::setRaw(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3)
{
    _print_func = RAW;
    
    _raw[0] = dig0;
    _raw[1] = dig1;
    _raw[2] = dig2;
    _raw[3] = dig3;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Display7seg4dig::setInteger(int16_t integer)
{
    if (integer < -999 || integer > 9999)
    {
         return STATUS_ERROR;
    }
    
    _print_func = INTEGER;
    
    _integer = integer;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Display7seg4dig::getBlink(uint16_t *period, uint8_t *duty_cycle)
{
    if (!period || !duty_cycle)
    {
        return STATUS_ERROR;
    }
    
    *period = _blink_period;
    *duty_cycle = _blink_duty_cycle;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Display7seg4dig::setBlink(uint16_t period, uint8_t duty_cycle)
{
    _blink_period = period;
    _blink_duty_cycle = duty_cycle;
    
    displaySetValue(_power);
    
    return STATUS_OK;
}

void Display7seg4dig::displaySetValue(bool value)
{
    if (value)
    {
        _display.setIntensity(_intensity);
        
        switch (_print_func)
        {
            case CHAR:
                _display.printString(&_char[0]);
                break;
                
            case RAW:
                _display.printRaw(&_raw[0]);
                break;
                
            case INTEGER:
                _display.printInteger(_integer);
                break;
            
            default:
                SERIAL_DEBUG.println("Unsupported Display 7seg 4dig private print function.");
        }
    }
    else
    {
        _display.clear();
    }
}
