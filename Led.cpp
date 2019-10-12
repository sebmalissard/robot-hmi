#include "Led.h"

typedef enum led_cmd_t
{
    POWER       = 0x01,
    INTENSITY   = 0x02,
    BLINK       = 0x03,
} led_cmd_t;

Led::Led(uint8_t pin, bool is_pwm): _pin(pin), _is_pwm(is_pwm), _power(LOW),  _intensity(128),
    _blink_period(0), _blink_duty_cycle(0)
{
    pinMode(pin, OUTPUT);
    
    ledSetValue(_power);
}

void Led::loop()
{
    static uint16_t i = 0;
    static bool last_value = LOW;
    
    if (_power)
    {
        if (i < DIV_ROUND_CLOSEST((uint32_t)_blink_period * _blink_duty_cycle / 255, LOOP_PERIOD))
        {
            if (last_value != HIGH)
            {
                ledSetValue(HIGH);
                last_value = HIGH;
            }
        }
        else
        {
            if (last_value != LOW)
            {
                ledSetValue(LOW);
                last_value = LOW;
            }
        }
        
        i++;
        
        if (i >= _blink_period / LOOP_PERIOD)
        {
            i = 0;
        }
    }
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
            
        case BLINK:
            *size = 3;
            return getBlink((uint16_t*) &arg[0], &arg[2]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Led get command.");
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
            
        case BLINK:
            if (size == 3)
                return setBlink((arg[0] << 8) + arg[1], arg[2]);
            break;
            
        default:
            SERIAL_DEBUG.println("Unsupported Led set command.");
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
    _power = power ? HIGH : LOW;
    
    ledSetValue(_power);
    
    return STATUS_OK;
}

status_t Led::getIntensity(uint8_t *intensity)
{
    if (!intensity)
    {
        return STATUS_ERROR;
    }
    
    *intensity = _intensity;
    
    return STATUS_OK;
}

status_t Led::setIntensity(uint8_t intensity)
{
    _intensity = intensity;
    
    ledSetValue(_power);
    
    return STATUS_OK;
}

status_t Led::getBlink(uint16_t *period, uint8_t *duty_cycle)
{
    if (!period || !duty_cycle)
    {
        return STATUS_ERROR;
    }
    
    *period = _blink_period;
    *duty_cycle = _blink_duty_cycle;
    
    return STATUS_OK;
}

status_t Led::setBlink(uint16_t period, uint8_t duty_cycle)
{
    _blink_period = period;
    _blink_duty_cycle = duty_cycle;
    
    ledSetValue(_power);
    
    return STATUS_OK;
}

void Led::ledSetValue(bool value)
{
    if (_is_pwm)
    {
        analogWrite(_pin, value ? _intensity : 0);
    }
    else
    {
        digitalWrite(_pin, value);
    }
}
