#include "Ax12.h"

#include <Wire.h>
#include "AX12A.h"

typedef enum ax12_cmd_t
{
    POWER_LED   = 0x01,
    POSITION    = 0x02,
} ax12_cmd_t;

Ax12::Ax12(uint8_t id): _force_update(false), _id(id), _power_led(LOW), _position(500)
{
    // Do not modify the initial position TODO return error unknown for get position in this case
    //ax12SetValue();
}

void Ax12::loop()
{   
    if (_force_update)
    {
        ax12SetValue();
        _force_update = false;
    }
}

status_t Ax12::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((ax12_cmd_t) cmd)
    {
        case ax12_cmd_t::POWER_LED:
            *size = 1;
            return getPowerLed(&arg[0]);
            
        case ax12_cmd_t::POSITION:
            *size = 2;
            return getPosition((uint16_t*) &arg[0]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Ax12 get command.");
    }

    return STATUS_INVALID_COMMAND;
}

status_t Ax12::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    switch ((ax12_cmd_t) cmd)
    {
        case ax12_cmd_t::POWER_LED:
             if (size == 1)
                return setPowerLed(arg[0]);
            else
                INVALID_ARGUMENT("Ax12 Power LED invalid number of argument.");
            
        case ax12_cmd_t::POSITION:
            if (size == 2)
                return setPosition((arg[0] << 8) + arg[1]);
            else
                INVALID_ARGUMENT("Ax12 position invalid number of argument.");
            
        default:
            SERIAL_DEBUG.println("Unsupported Ax12 set command.");
    }
    
    return STATUS_INVALID_COMMAND;
}

status_t Ax12::getPowerLed(uint8_t *power_led)
{
    if (!power_led)
    {
        return STATUS_ERROR;
    }
    
    *power_led = _power_led;
    
    return STATUS_OK;
}

status_t Ax12::setPowerLed(uint8_t power_led)
{
    _power_led = power_led ? HIGH : LOW;
    
    _force_update = true;
    
    return STATUS_OK;
}

status_t Ax12::getPosition(uint16_t *position)
{
    if (!position)
    {
        return STATUS_ERROR;
    }
    
    *position = _position;
    
    return STATUS_OK;
}

status_t Ax12::setPosition(uint16_t position)
{
    _position = position;
    
    _force_update = true;
    
    return STATUS_OK;
}

void Ax12::ax12SetValue()
{
    ax12a.move(_id, _position);
    ax12a.ledStatus(_id, _power_led);
}
